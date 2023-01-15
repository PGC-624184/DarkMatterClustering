# Dark Matter Clustering

Friends of Friends is a piece of cosmological simulation analysis software written as part of a university project in C. There are eventual plans to port this code to [Julia 1.8](https://julialang.org/downloads/) for ease of development.

The makefile currently exists within the /src folder, until I can figure out how to move it back into the main folder. The data required to run the simulation is not available for public release (from my knowledge), but I have included the format required from the code.

Enjoy and remember, faster is better in the wild west 🤠

Data is in a .txt file  with the following first line:
| Number of Particles | Box Size |
| ------------------- | -------|
| 329072 | 14285.714 |

And the subsequent particles as;

| X | Y | Z | Mass | Type |
| --------- | --------- | --------- | --------- | ----- |
| 12.592805 | 833.43500 | 309.95242 | 1.1582964E+08 | 0 | 
| 584.23944 | 442.12723 | 102.96899 | 1.1582964E+08 | 0 |
| 657.83441 | 217.12383 | 530.37024 | 1.1582964E+08 | 0 | 
| 666.80109 | 659.12408 | 794.49884 | 1.1582964E+08 | 0 | 
| 680.55652 | 613.83008 | 1102.3507 | 1.1582964E+08 | 0 | 
| 692.23657 | 578.98187 | 1372.2134 | 1.1582964E+08 | 0 | 
