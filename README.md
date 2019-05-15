# clsd
An c++ implementation of the LSD algorithm to detect super bubbles.


##Install

To install:
1. Clone git repro.
2. Run configure.
3. Run make.
```
    git clone git@github.com:Fabianexe/clsd.git
    ./configure 
    make
```
##Statistics

clsd can compute different stastistics.
Notet that clsd ignores multi edges. Thus every stastistic is done by ignoring multiple appearence of the same edge.
The output is a list of numbers sperated by &.
Before content line is header line. The header shorts mean:

### Graph Properties
| Metric                      | Short       | Paper    | Description    |
|:---------------------------:|:-----------:|:--------:|:--------------:|
|Vertices                     | N           | -        | Number of Vertices  |
|Edges                        | M           | -        | Number of Edges  |
|Multi Edges                  | ME          | -        | Fraction of Edges that are multi edges |
|Max Degree                   | deg         | -        | The largest degree  |
|Graph Density                | GD          | -        | (2m)/(n(n-1))  |
|Connected components         | CC          | -        | The number of connected components|
|Assortativity                | R           | [Mixing patterns in networks](https://journals.aps.org/pre/pdf/10.1103/PhysRevE.67.026126) | Valiation of euqal degrees on neighbor vertices 
|Self-Similarity              | SS          | [Towards a Theory of Scale-Free Graphs: Definition, Properties, and Implications](https://projecteuclid.org/download/pdf_1/euclid.im/1150477667) | This is maximized when high-degree nodes are connected to other high-degree nodes


### Digraph Properties
| Metric                      | Short       | Paper    | Description    |
|:---------------------------:|:-----------:|:--------:|:--------------:|
|Max in Degree                | $deg_\\leftarrow$       | -        | The largest inDegree  |
|Max out Degree               | $deg_\\rightarrow$      | -        | The largest outDegree  |
|Strongly connected components| SCC         | -        | The number of strongly connected components|
|Bidirectional edges          | BE          | -        | Fraction of the edges that have invertet edge in Each
|Directed assortativity       | $R_{\\leftarrow\\leftarrow}$        | [Edge direction and the structure of networks](https://www.pnas.org/content/pnas/107/24/10815.full.pdf) | Valiation of euqal degrees on neighbor vertices by comparing in- to in-degree
|Directed assortativity       | $R_{\\leftarrow\\rightarrow}$      | [Edge direction and the structure of networks](https://www.pnas.org/content/pnas/107/24/10815.full.pdf) | Valiation of euqal degrees on neighbor vertices by comparing in- to out-degree
|Directed assortativity       | $R_{\\rightarrow\\leftarrow}$      | [Edge direction and the structure of networks](https://www.pnas.org/content/pnas/107/24/10815.full.pdf) | Valiation of euqal degrees on neighbor vertices by comparing out- to in-degree
|Directed assortativity       | $R_{\\rightarrow\\rightarrow}$     | [Edge direction and the structure of networks](https://www.pnas.org/content/pnas/107/24/10815.full.pdf) | Valiation of euqal degrees on neighbor vertices by comparing out- to out-degree
|Heterogeneity index          | H           | [Entropy and Heterogeneity Measures for Directed Graphs](https://link.springer.com/chapter/10.1007/978-3-642-39140-8_15) | Entropy of neighbor degrees 

### Superbubble Properties
| Metric                      | Short        | Description    |
|:---------------------------:|:------------:|:--------------:|
| Number of Superbubbles      | S            | How many superbubbles in the graph
| Vertices Superbubbles       | VS           | The fraction of vertices that are part of a superbubble
| Edges Superbubbles          | ES           | The fraction of edges that are part of a superbubble
| Number of Mini-Superbubbles | MS           | How many mini superbubbles in the graph i.e. superbubbles with two vertices
| Max Vertices                | mVS          | What is the maximum of vertices in one superbubble
| Max Edges                   | mES          | What is the maximum of edges in one superbubble
| Superbubble Complex         | C            | How many non overlapping superbubble complexes exists
| Superbubble Complex size    | CS           | The maximum of superbubbles in one complex
| Deepest Superbubble         | depth        | The maximal deep that a superbubble have
| maximal paths in Superbubble| P            | The maximal number of a paths in one superbubble
| longest path in Superbubble | PL           | The maximal length of a path in a superbubble

#### Non-mini Superbubble Properties
| Metric                      | Short        | Description    |
|:---------------------------:|:------------:|:--------------:|
| Average number of paths     | aP           | The average number of paths in non mini superbubbles
| Average path lenght         | aPL          | The average path length  in non mini superbubbles
| Superbubble Density         | SD           | The average value of (2m)/(n(n-1)) for all superbubbles  in non mini superbubbles
