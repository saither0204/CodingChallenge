#!/Users/saither/miniforge3/envs/env2/bin/python
import sys
import os
import argparse
from my_lexer import Lexer, LexerError
from my_parser import JSONParser, JSONParserError

def process_file(filepath):
    """Process a single file and return True if valid JSON, False otherwise."""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            text = f.read()
    except Exception as e:
        sys.stderr.write(f"Error reading file {filepath}: {e}\n")
        return False

    lexer = Lexer(text)
    parser_instance = JSONParser(lexer)

    try:
        _ = parser_instance.parse()
        return True
    except (LexerError, JSONParserError) as e:
        sys.stderr.write(f"Invalid JSON in {filepath}: {e}\n")
        return False

def process_directory(directory):
    """Process all .json files in the directory and print a summary."""
    summary = {"passed": [], "failed": []}
    # Iterate over the files in the directory (non-recursive)
    for filename in os.listdir(directory):
        if filename.lower().endswith('.json'):
            filepath = os.path.join(directory, filename)
            valid = process_file(filepath)
            if valid:
                summary["passed"].append(filename)
            else:
                summary["failed"].append(filename)
    return summary

def main():
    arg_parser = argparse.ArgumentParser(
        description="A custom JSON parser CLI tool. Accepts a file or a directory."
    )
    arg_parser.add_argument("json_path", nargs="?", help="JSON file or directory to parse (reads from stdin if not provided)")
    args = arg_parser.parse_args()
    
    # No argument: read from stdin
    if not args.json_path:
        text = sys.stdin.read()
        lexer = Lexer(text)
        parser_instance = JSONParser(lexer)
        try:
            _ = parser_instance.parse()
            sys.stdout.write("Valid JSON\n")
            sys.exit(0)
        except (LexerError, JSONParserError) as e:
            sys.stderr.write(f"Invalid JSON: {e}\n")
            sys.exit(1)

    # If the argument is a directory
    if os.path.isdir(args.json_path):
        summary = process_directory(args.json_path)
        sys.stdout.write("JSON Directory Report:\n")
        sys.stdout.write("Passed Files:\n")
        for file in summary["passed"]:
            sys.stdout.write(f"  {file}\n")
        sys.stdout.write("Failed Files:\n")
        for file in summary["failed"]:
            sys.stdout.write(f"  {file}\n")
        # Exit with error code if any file failed
        sys.exit(1 if summary["failed"] else 0)
    # If the argument is a file
    elif os.path.isfile(args.json_path):
        valid = process_file(args.json_path)
        if valid:
            sys.stdout.write(f"{args.json_path}: Valid JSON\n")
            sys.exit(0)
        else:
            sys.stderr.write(f"{args.json_path}: Invalid JSON\n")
            sys.exit(1)
    else:
        sys.stderr.write(f"Error: {args.json_path} is not a valid file or directory.\n")
        sys.exit(1)

if __name__ == "__main__":
    main()
