# Huffman-Encoding-Decoding

## Steps to use the package:

1. Clear any text/data from "source.txt", "destination.txt", "header.huffh" and "body.huffb" before using
2. Write your text in "source.txt"
3. Run "compressor.cpp"
4. At this point you can see that "header.huffh" and "body.huffb" have been written into; that is the compressed text
5. Run "decompressor.cpp"
6. Now you can see the decompressed text in "destination.txt"

## Introduction

In today's internet-driven era, efficient file transmission is crucial to save bandwidth and reduce network usage. To achieve this, compressing or encoding files to minimise their size is essential. One widely used method for compressing text files is Huffman encoding. This technique assigns variable-length binary codes to each character, ensuring that no code is a prefix of another. The most common characters receive shorter codes, while less frequent ones get longer codes. However, assigning these codes can be a challenging task.

## Huffman Encoding Algorithm and Huffman Tree

The initial step of Huffman encoding involves constructing a Huffman tree using the provided characters. This tree allows us to assign codes to each character, which will be utilised for compression.

The following are the steps to make a Huffman tree:

```
For each unique character in the source text:

	Make a leaf node in the tree storing the frequency of the character

Build a min-heap of nodes, which compares based on the frequency of the nodes

while the size of the head is greater than one:

	Extract 2 minimum nodes from the heap

	Create a node with frequency as sum of frequencies of extracted nodes
	
	Set extracted nodes as left and right children of newly created node

	Insert the new node in the min-heap
```

After these steps, the Huffman tree is complete, and the last node is treated as the root of the tree.

The second part is to assign codes based on the Huffman tree. To do so, we do the following:

```
Start at the root node, and maintain a binary string (initially empty)

Perform DFS on the tree starting from the root

If left child is being visited:
	0 is appended to the binary string
	
If right child is being visited:
	1 is appended to the binary string
	
If DFS is completed on a node and the node is being exited:
	The binary string’s last character is popped

If we reach a leaf node (associated with a character):
	The Huffman code of that character is the binary string
```

The Huffman encoding method satisfies the following objectives:

1. The lengths of character codes are variable, with more frequent characters having shorter codes.
2. No character code is a prefix of another character code.
3. The Huffman tree reflects the frequency of characters, placing more frequent characters closer to the root. As a result, more frequent characters obtain shorter codes, fulfilling the first objective.

For the second objective, the character code is determined by the path from the root to the respective node. If the code for character A needs to be a prefix for character B, A must lie on the path from the root to B. However, since characters are stored in leaf nodes, the path to character B cannot contain the node for character A. This ensures that the prefix code condition is met.

### Note

The algorithm encounters an edge case when the text file contains only one character. In such situations, the character is left without a binary code if the standard encoding process is followed.

However, this edge case is handled during the decoding process. The header file, which contains character frequencies, helps identify the scenario where the file consists of a single character repeated multiple times. This information allows the decoding program to reconstruct the original file correctly.

## Huffman Decoding Algorithm

Huffman decoding is the process of converting a compressed file back to its original form based on the Huffman encoding scheme. It involves using the Huffman tree, constructed during the encoding process, to decode the compressed binary data.

Here are the steps:

```
Start at the root node, and maintain a decoded string (initially empty)

For each bit in encoded data:

  If bit is 0:
    Go to left child of current node

  Else:
    Go to right child of current node

  If current node is a leaf node:
    Append the character represented by this node to the decoded string

The decoded string represents the original text file
```

## Conclusion

The Huffman Encoding implemented here achieves up to 30% data compression, demonstrating the effectiveness of the Huffman algorithm for lossless data compression. The Huffman Decoding successfully retrieves the original data ensuring lossless decompression.
