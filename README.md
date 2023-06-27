# A mesh simplification algorithm based on iterative edge contraction
![initial mesh](https://i.imgur.com/HXTQ9VP.png) ![reduced mesh](https://i.imgur.com/Fwm0GuE.png)

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
After an edge operation, vertex positions are tweaked so that the reduction's surface matches the initial one as much as possible. Optimal positions can be computed using the linear least squares method.

It's then possible to define the global principle of the algorithm: edges are randomly chosen, and random operations are performed on them. An operation is kept only if it makes the energy decrease. This process goes on until the energy function or the reduction factor reaches a chosen threshold.

## Implementation
The implementation is still being worked on, the operations on vertex positions being currently implemented using the librairies `OpenBLAS` and `LAPACKE` (see [wip](https://github.com/jonas-trms/edge_contraction/tree/wip)). There is currently a problem with these operations, because `LAPACKE` solves the linear least squares subproblem using the wrong matrix norm, providing therefore bad positions for vertices. A new computation using the conjugate gradient method is to be implemented. 

For now, without these vertex operations, the energy function is basically useless, because it never decreases. Consequently, edges are randomly collapsed, until the desired reduction factor is reached.

The execution is quite slow and could be accelerated using heuristics. Nevertheless, this algorithm has the advantage of preserving the topological details of the mesh. Its global error isn't very good yet, because of the unimplemented operations.

 ## Compilation
 You can compile with `gcc` by running the following commands:
```
$ gcc -o edge_contraction src/main.c src/access_operations.c src/comparison.c src/condition_tests.c src/edge_operations.c src/in-out.c src/linked_lists.c src/remove_operations.c -lm
$ gcc -o error src/energy.c src/access_operations.c src/comparison.c src/condition_tests.c src/edge_operations.c src/in-out_energy.c src/linked_lists.c src/projection.c src/remove_operations.c -lm
```

## Usage
The algorithm only supports triangular meshes and this implementation expects `.obj` files formatted in a specific way (see [Format](#format)). There are two commands:
* `./edge_contraction [args]` reduces the provided mesh using the described algorithm. Arguments:
  * `initial` is the path to the `.obj` file to be reduced.
  * `reduction` is the path to the reduced `.obj` file.
  * `factor` is a `float` value equal to the desired reduction factor.

  Example:
  ```
  $ ./edge_contraction head.obj head_reduced.obj 2.
  ```

* `./error [args]` computes and prints the error (a float value) of a mesh's reduction. Arguments:
  * `initial` is the path to the initial `.obj` file.
  * `reduction` is the path to the reduced `.obj` file.
  * `k` is a `float` value and defines the importance of smoothness to the quality of the reduction. A higher value means that smoothness is more important.
  * `crep` is a `float` value and defines the balance between the importance of a low number of vertices and of a faithful surface. A higher value means that the former is more important.

  Example:
  ```
  $ ./error head.obj head_reduced.obj 0. 0.
  ```

## Format
The `.obj` files must be formatted in a specific way. The first line should be the following comment:
```#n vertices; m faces```.

Then, the file should only contain the vertex positions and the faces. Faces must be triplets of integers. For the line breaks, see the example.

Example of a formatted file:
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
## Attributions
The images of the edge operations come from [_Mesh optimization_](https://doi.org/10.1145/166117.166119).
