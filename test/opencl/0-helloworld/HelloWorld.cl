__kernel void helloworld(
    __global const float *a,
    __global const float *b,
    __global float *result,
    const unsigned long count)
{
    unsigned long idx = get_global_id(0);
    if (idx < count) {
        result[idx] = 0.0;
        float coeff = 1.0 / (float)(count);
        for (unsigned long j = 0; j < 100; ++j) {
            result[idx] = a[idx]*coeff + b[idx]*coeff;
        }
    }
}
