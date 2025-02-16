#!/usr/bin/env python3
import sys
import argparse
from my_lexer import Lexer, LexerError
from my_parser import JSONParser, JSONParserError

def main():
    # Use a unique name for the ArgumentParser instance.
    arg_parser = argparse.ArgumentParser(
        description="A custom JSON parser CLI tool."
    )
    arg_parser.add_argument("json_file", nargs="?", help="JSON file to parse (reads from stdin if not provided)")
    args = arg_parser.parse_args()
    
    # Read input from the provided file or from standard input.
    if args.json_file:
        try:
            with open(args.json_file, 'r', encoding='utf-8') as f:
                text = f.read()
        except Exception as e:
            sys.stderr.write(f"Error reading file {args.json_file}: {e}\n")
            sys.exit(1)
    else:
        text = sys.stdin.read()
    
    # Create a lexer and parser instance with unique names.
    lexer = Lexer(text)
    parser_instance = JSONParser(lexer)
    
    try:
        result = parser_instance.parse()
        sys.stdout.write("Valid JSON\n")
        # Uncomment the next line for debugging output:
        # sys.stdout.write(str(result))
        sys.exit(0)
    except (LexerError, JSONParserError) as e:
        sys.stderr.write(f"Invalid JSON: {e}\n")
        sys.exit(1)

if __name__ == "__main__":
    main()
