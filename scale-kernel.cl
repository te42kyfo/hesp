__kernel void scale(__global real* data, real scalar) {
    int idx = get_global_id(0);
    data[idx] = data[idx] * scalar;
}
