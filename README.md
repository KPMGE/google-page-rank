# Google page rank

### Conventions

#### Code style
- [x] Code in English.
- [x] Functions must be written in *snake_case*.
- [x] _Structs_ must start with the first letter in uppercase and use _CamelCase_.
- [x] The **.h** files must be at the *include* folder.
- [x] The **.c** files must be at the _src_ folder.

#### Commit style
- [x] Commits in English.
- [x] Commits with emojis from [gitmoji](https://gitmoji.dev/).


### Data structures
1. We'll use a [red-black-tree](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree) to store a list with our _stopwords_.
2. We'll need to use a graph to represent the pages their relationship.
  - Each node is gonna have the page name, and two sets representing the incoming and 
    outgoing pages relative to the current one. Finally, we must store the page rank, which 
    in turn, 
  - The *page rank* for each page must be initialized with *1/N*, where *N* is the total amount of pages.
3. We can store the pages in a [red-black-tree](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree) and 
  then store the links in an adjacent list using either an array or a linked-list.
