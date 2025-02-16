from tokens import *

class LexerError(Exception):
    pass

class Lexer:
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_char = self.text[self.pos] if self.text else None
    
    def error(self, msg):
        raise LexerError(msg)
    
    def advance(self):
        """Advance the position and update the current character."""
        self.pos += 1
        if self.pos >= len(self.text):
            self.current_char = None
        else:
            self.current_char = self.text[self.pos]
    
    def skip_whitespace(self):
        while self.current_char is not None and self.current_char in " \t\n\r":
            self.advance()
    
    def string(self):
        """Parse a JSON string (assumes starting quote is consumed)."""
        result = ""
        while True:
            if self.current_char is None:
                self.error("Unterminated string")
            if self.current_char == '"':
                self.advance()  # closing quote
                break
            if self.current_char == '\\':  # handle escape sequences
                self.advance()
                if self.current_char in ['"', '\\', '/', 'b', 'f', 'n', 'r', 't']:
                    result += '\\' + self.current_char
                else:
                    self.error("Invalid escape sequence")
            else:
                result += self.current_char
            self.advance()
        return (TOKEN_STRING, result)
    
    def number(self):
        """Parse a number."""
        result = ""
        while self.current_char is not None and (self.current_char.isdigit() or self.current_char in '.-+eE'):
            result += self.current_char
            self.advance()
        return (TOKEN_NUMBER, result)
    
    def next_token(self):
        while self.current_char is not None:
            if self.current_char in " \t\n\r":
                self.skip_whitespace()
                continue
            
            if self.current_char == '{':
                self.advance()
                return (TOKEN_LBRACE, '{')
            if self.current_char == '}':
                self.advance()
                return (TOKEN_RBRACE, '}')
            if self.current_char == '[':
                self.advance()
                return (TOKEN_LBRACKET, '[')
            if self.current_char == ']':
                self.advance()
                return (TOKEN_RBRACKET, ']')
            if self.current_char == ':':
                self.advance()
                return (TOKEN_COLON, ':')
            if self.current_char == ',':
                self.advance()
                return (TOKEN_COMMA, ',')
            if self.current_char == '"':
                self.advance()  # consume the opening quote
                return self.string()
            if self.current_char.isdigit() or self.current_char in '-':
                return self.number()
            # Handle literals: true, false, null
            if self.text[self.pos:self.pos+4] == 'true':
                self.pos += 4
                self.current_char = self.text[self.pos] if self.pos < len(self.text) else None
                return (TOKEN_TRUE, True)
            if self.text[self.pos:self.pos+5] == 'false':
                self.pos += 5
                self.current_char = self.text[self.pos] if self.pos < len(self.text) else None
                return (TOKEN_FALSE, False)
            if self.text[self.pos:self.pos+4] == 'null':
                self.pos += 4
                self.current_char = self.text[self.pos] if self.pos < len(self.text) else None
                return (TOKEN_NULL, None)
            
            self.error(f"Unexpected character: {self.current_char}")
        
        return (TOKEN_EOF, None)