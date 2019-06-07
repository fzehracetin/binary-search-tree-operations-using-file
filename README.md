# Binary Search Tree Operations Using File

A social network website keeps users's informations in binary search tree structure. Here is all operations that uses Binary Search Tree.

Tree is gonna built from [Input.txt](https://github.com/fzehracetin/binary-search-tree-operations-using-file/blob/master/Input.txt) file. Every line of that file consists user's id, user's name, user's friends's id's.

#### Line Example: 
31,Ayse Terim,9-15-53-60
id, name surname, id-id-id-id

Functions in the program listed below:

1. **addAll() :** Adds all people from file.

2. **insertNewUser() :** Adds new user to tree. Takes user's id as an input. Id must exist in the file.

3. **deleteUser() :**  Deletes user from tree. Takes user's id as an input. Id must exist in the bst.

4. **contains() :** Searches user from tree. Takes user's id as an input. Prints the user's name and surname.

5. **friends() :** Prints the user's friends's name and surname. Takes user's id as an input.

6. **size() :** Prints the number of nodes in the tree.

7. **printNext() :** Prints user's subtree in ascending order. Takes user's id as an input.

8. **printInOrder() :** Prints all nodes in the tree in order.

9. **printGreater() :** Prints the users's name-surname and id that has greater id from given user's id. Takes user's id as an input. 
