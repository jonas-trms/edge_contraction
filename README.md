# A mesh simplification algorithm based on _iterative edge contraction_
This algorithm is based on the paper _Mesh optimization_ by H.HOPPE, T.DEROSE, T.DUCHAMP, J.MCDONALD, W.STUETZLE

Edges are chosen successively and their incident vertices are merged, so as to minimize an energy function.

The implementation is still being worked on, the energy function not being used yet. Edges are therefore randomly collapsed, until the desired reduction factor is reached. \
The execution is quite slow, but this algorithm has the advantage of preserving the topological details of the mesh. Its global error isn't very good yet, because of the unused energy function.

# Execution
Meshes must be in obj format

**main.c** : reduces a mesh \
Arguments : input path, output path, desired reduction factor

**energy.c** : computes the error (a float value) between a mesh and one of its simplifications \
Arguments : initial mesh path, simplified mesh path

# Example
![](https://i.imgur.com/c4XOg8p.png) ![](https://i.imgur.com/z7ff6Wx.png) \
The number of vertices was divided by 4
