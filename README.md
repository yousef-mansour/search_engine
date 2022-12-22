# search_engine
![image](https://user-images.githubusercontent.com/98807490/209160138-b7a09279-dde0-4527-ac75-7c5db501e613.png)<br>
A simple search engine that assign score to webpages using click through rates, impressions, and probability distribution in the Markov chain.

### The input files of the program:
- Web graph file (in CSV format). Each line in the input file would have two URLs showing a link from the first URL page to the second URL page. 
- Keyword file (in CSV format), which contains the list of keywords for each webpage. 
- Number of impressions file (in CSV format), which contains the initial number of times each webpage appeared in the search results (needed to compute Click-through rate). 
