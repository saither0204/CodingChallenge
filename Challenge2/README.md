# JSON Parser

A Python JSON parser that converts JSON data into Python objects.

## Features

- Parse JSON strings
- Handle nested objects and arrays
- Validate JSON format

## Usage

```python
from json_parser import parse

json_string = '{"name": "John", "age": 30, "city": "New York"}'
parsed_data = parse(json_string)
print(parsed_data)
```

## Example

```python
from json_parser import parse

json_string = '''
{
    "name": "John",
    "age": 30,
    "city": "New York",
    "children": [
        {"name": "Anna", "age": 10},
        {"name": "Alex", "age": 8}
    ]
}
'''

parsed_data = parse(json_string)
print(parsed_data)
```

## Testing

```bash
python -m unittest discover tests
```
