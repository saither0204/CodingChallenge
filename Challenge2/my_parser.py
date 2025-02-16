from tokens import *

class JSONParserError(Exception):
    pass

class JSONParser:
    def __init__(self, lexer, max_depth=20):
        self.lexer = lexer
        self.current_token = self.lexer.next_token()
        self.max_depth = max_depth

    def error(self, msg):
        raise JSONParserError(msg)

    def eat(self, token_type):
        if self.current_token[0] == token_type:
            self.current_token = self.lexer.next_token()
        else:
            self.error(f"Expected token {token_type} but got {self.current_token[0]}")

    def parse(self):
        # Check that the first token is either a left brace or left bracket
        if self.current_token[0] not in (TOKEN_LBRACE, TOKEN_LBRACKET):
            self.error("A JSON payload should be an object or array, not a " + self.current_token[0])
        result = self.value(0)
        if self.current_token[0] != TOKEN_EOF:
            self.error("Extra data after valid JSON")
        return result

    def value(self, depth):
        # Use a strict check: if depth is equal to max_depth, error out.
        if depth >= self.max_depth:
            self.error("Nesting too deep")
        token_type, token_value = self.current_token
        if token_type == TOKEN_LBRACE:
            return self.object(depth + 1)
        elif token_type == TOKEN_LBRACKET:
            return self.array(depth + 1)
        elif token_type == TOKEN_STRING:
            self.eat(TOKEN_STRING)
            return token_value
        elif token_type == TOKEN_NUMBER:
            self.eat(TOKEN_NUMBER)
            try:
                if '.' in token_value or 'e' in token_value or 'E' in token_value:
                    return float(token_value)
                else:
                    return int(token_value)
            except ValueError:
                self.error("Invalid number format")
        elif token_type == TOKEN_TRUE:
            self.eat(TOKEN_TRUE)
            return True
        elif token_type == TOKEN_FALSE:
            self.eat(TOKEN_FALSE)
            return False
        elif token_type == TOKEN_NULL:
            self.eat(TOKEN_NULL)
            return None
        else:
            self.error(f"Unexpected token {token_type}")

    def object(self, depth):
        result = {}
        self.eat(TOKEN_LBRACE)
        if self.current_token[0] == TOKEN_RBRACE:
            self.eat(TOKEN_RBRACE)
            return result
        while True:
            if self.current_token[0] != TOKEN_STRING:
                self.error("Object keys must be strings")
            key = self.current_token[1]
            self.eat(TOKEN_STRING)
            self.eat(TOKEN_COLON)
            result[key] = self.value(depth + 1)
            if self.current_token[0] == TOKEN_COMMA:
                self.eat(TOKEN_COMMA)
                continue
            elif self.current_token[0] == TOKEN_RBRACE:
                self.eat(TOKEN_RBRACE)
                break
            else:
                self.error("Expected ',' or '}' in object")
        return result

    def array(self, depth):
        result = []
        self.eat(TOKEN_LBRACKET)
        if self.current_token[0] == TOKEN_RBRACKET:
            self.eat(TOKEN_RBRACKET)
            return result
        while True:
            # Each element in the array is at the current depth (no additional increment here)
            result.append(self.value(depth))
            if self.current_token[0] == TOKEN_COMMA:
                self.eat(TOKEN_COMMA)
                continue
            elif self.current_token[0] == TOKEN_RBRACKET:
                self.eat(TOKEN_RBRACKET)
                break
            else:
                self.error("Expected ',' or ']' in array")
        return result
