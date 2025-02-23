## Pseudo Code for the project (Try to make it in C++)

```
function countFrequencies(filename):
    if file does not exist:
        return error
    frequencyMap = new Map()
    for each character in file:
        if character in frequencyMap:
            frequencyMap[character] += 1
        else:
            frequencyMap[character] = 1
    return frequencyMap
```