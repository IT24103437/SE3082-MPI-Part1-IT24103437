## Exercise 6: Observation

Exercise 6 produced the same point counts and Pi estimates as Exercise 3 for each process count. Using `MPI_ANY_SOURCE` changed how rank 0 received the counts, but did not change the calculation. With four processes, the counts arrived from ranks 1, 3, and 2, showing that rank 0 accepted whichever message was ready.

| Processes | Exercise 3 time (s) | Exercise 6 time (s) |
|---:|---:|---:|
| 1 | 0.183639 | 0.184232 |
| 2 | 0.117523 | 0.120202 |
| 4 | 0.055292 | 0.059015 |

Exercise 6 was slightly slower in these single runs. More runs would be needed to determine whether that difference is consistent.