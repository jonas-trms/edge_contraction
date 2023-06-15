# A mesh simplification algorithm based on iterative edge contraction
This algorithm is based on the paper [_Mesh optimization_](https://doi.org/10.1145/166117.166119) by H. Hoppe, T. Derose, T. Duchamp, J. McDonald, W. Stuetzle.

Edges are chosen successively and their incident vertices are merged, so as to minimize an energy function.

The implementation is still being worked on, the energy function not being used yet. Edges are therefore randomly collapsed, until the desired reduction factor is reached.

The execution is quite slow, but this algorithm has the advantage of preserving the topological details of the mesh. Its global error isn't very good yet, because of the unused energy function.

 # Compilation
 You can compile with GCC by running the following commands:
```
$ gcc -o main main.c access_operations.c comparison.c conditions_tests.c edge_operations.c in-out.c linked_lists.c remove_operations.c -lm
```

```
$ gcc -o energy access_operations.c comparison.c conditions_tests.c edge_operations.c in-out_energy.c linked_lists.c projection.c remove_operations.c -lm
```

# Execution
Meshes must be in OBJ format.

## main.c
Reduces a mesh.

Arguments: input path, output path, desired reduction factor.

Example:
```
$ ./main head.obj head_reduced.obj 2.
```

## energy.c
Computes the error (a float value) between a mesh and one of its simplifications.

Arguments: initial mesh path, simplified mesh path, _k_, _crep_ (_cf. [Mesh optimization](https://doi.org/10.1145/166117.166119)_).

Example:
```
$ ./energy head.obj head_reduced.obj 0. 0.
```

# Example
![initial mesh](https://i.imgur.com/c4XOg8p.png) ![reduced mesh](https://i.imgur.com/z7ff6Wx.png) \
The number of vertices was divided by 2 (the initial mesh is on the left), and the error of the reduction is 0.23.
