# A mesh simplification algorithm based on iterative edge contraction
![initial mesh](https://i.imgur.com/c4XOg8p.png) ![reduced mesh](https://i.imgur.com/z7ff6Wx.png)

This algorithm is based on the paper [_Mesh optimization_](https://doi.org/10.1145/166117.166119) by H. Hoppe, T. Derose, T. Duchamp, J. McDonald, W. Stuetzle.

## Principle
Operations are iteratively performed on edges, so as to minimize an energy function measuring the distance between the initial mesh and the reduced one. 

### Edge operations
For a chosen edge, three operations are allowed: an edge can be collapsed, rotated or split.

#### Edge collapse
![initial situation](https://i.imgur.com/GUarpWn.png) ![collapse operation](https://i.imgur.com/srvrpOq.png) \
This operation is the one that actually reduces the mesh, as it reduces the number of vertices.

#### Edge rotation
![initial situation](https://i.imgur.com/GUarpWn.png) ![pivot operation](https://i.imgur.com/pD7kvtz.png) \
This operation helps matching the original mesh.

#### Edge split
![initial situation](https://i.imgur.com/GUarpWn.png) ![vertex creation](https://i.imgur.com/io3a8Ot.png) \
This operation prevents the appearance of sharp spikes, and helps matching the original mesh.

### Global algorithm
After an edge operation, vertex positions are tweaked so that the reduction's surface matches the initial one as much as possible.

Edges are then randomly chosen, and random operations are performed on them. An operation is kept if it makes the energy decrease. This process goes on until the energy function, or the reduction factor, reaches a chosen threshold.

## Implementation
The current implementation is still being worked on, operations on vertex positions not being implemented yet. The energy function is therefore useless, because it never decreases. Consequently, edges are randomly collapsed, until the desired reduction factor is reached.

The execution is quite slow, and could be fastened using heuristics. Nevertheless, this algorithm has the advantage of preserving the topological details of the mesh. Its global error isn't very good yet, because of the unimplemented operations.

 ## Compilation
 You can compile with GCC by running the following commands:
```
$ gcc -o main main.c access_operations.c comparison.c conditions_tests.c edge_operations.c in-out.c linked_lists.c remove_operations.c -lm
$ gcc -o energy access_operations.c comparison.c conditions_tests.c edge_operations.c in-out_energy.c linked_lists.c projection.c remove_operations.c -lm
```

## Usage
The algorithm only supports triangular meshes and this implementation expects `.obj` files formated in a specific way (see [Format](#format)). There are two commands:
* `./main [args]` reduces the provided mesh using the described algorithm. Arguments:
  * `initial` is the path to the `.obj` file to be reduced.
  * `reduction` is the path to the reduced `.obj` file.
  * `factor` is a `float` value and is equal to the desired reduction factor.

  Example:
  ```
  $ ./main head.obj head_reduced.obj 2.
  ```

* `./energy [args]` computes and prints the error (a float value) of a mesh's reduction. Arguments:
  * `initial` is the path to the initial `.obj` file.
  * `reduction` is the path to the reduced `.obj` file.
  * `k` is a `float` value and defines the importance of smoothness to the quality of the reduction.
  * `crep` is a `float` value and defines the balance between the importance of a low number of vertices and of a faithful surface.

  Example:
  ```
  $ ./energy head.obj head_reduced.obj 0. 0.
  ```

## Format
The `.obj` files must be formated in a specific way. The first line should be the following comment :
```#n vertices; m faces```

Then, the file should only contain the vertex positions and the faces. Faces must be triplets of integers. For the line breaks, see the example.

Example of a formated file:
```obj
#8 vertices; 12 faces

v -10.000000 -10.000000 -10.000000
v -10.000000 -10.000000 10.000000
v -10.000000 10.000000 10.000000
v -10.000000 10.000000 -10.000000
v 10.000000 -10.000000 -10.000000
v 10.000000 -10.000000 10.000000
v 10.000000 10.000000 10.000000
v 10.000000 10.000000 -10.000000

f 1 4 3
f 1 2 3
f 1 5 6
f 1 2 6
f 1 5 8
f 1 4 8
f 5 8 7
f 5 6 7
f 3 8 7
f 4 3 8
f 2 6 7
f 2 3 7
```
