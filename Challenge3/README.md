# Pseudo Codes for the project (Try to make it in C++)

## Read File and Count Frequencies

```cpp
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

## Build the Huffman Tree

```cpp
function buildHuffmanTree(frequencyMap):
    priorityQueue = new PriorityQueue()
    for character, frequency in frequencyMap:
        node = new TreeNode(character, frequency)
        priorityQueue.insert(node)
    while priorityQueue.size() > 1:
        left = priorityQueue.removeMin()
        right = priorityQueue.removeMin()
        mergedNode = new TreeNode(null, left.frequency + right.frequency)
        mergedNode.left = left
        mergedNode.right = right
        priorityQueue.insert(mergedNode)
    return priorityQueue.removeMin()  // This is the root of the Huffman tree.
```

## Generate the Prefix-Code Table

```cpp
function generateCodes(node, currentCode, codeTable):
    if node is a leaf:
        codeTable[node.character] = currentCode
        return
    if node.left is not null:
        generateCodes(node.left, currentCode + "0", codeTable)
    if node.right is not null:
        generateCodes(node.right, currentCode + "1", codeTable)

function getPrefixCodes(root):
    codeTable = new Map()
    generateCodes(root, "", codeTable)
    return codeTable
```

## Write the Header to the Output File

## Encode the Text

```cpp
function encodeText(text, codeTable):
    bitString = ""
    for character in text:
        bitString += codeTable[character]
    return bitString

function packBits(bitString):
    // Convert the bitString to an array of bytes.
    // Handle padding if necessary.
    return byteArray
```

## Read the Header and Rebuild the Prefix Table

## Decode the Compressed Data

```cpp
function decodeBits(byteArray, root, totalBits):
    currentNode = root
    decodedText = ""
    bitCount = 0
    for each bit in byteArray (up to totalBits):
        if bit == '0':
            currentNode = currentNode.left
        else:
            currentNode = currentNode.right
        if currentNode is a leaf:
            decodedText += currentNode.character
            currentNode = root
        bitCount += 1
    return decodedText
```
