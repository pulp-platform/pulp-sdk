
// Parameters are declared in this order:
// size, length, src_stride_2d, dst_stride_2d, src_stride_3d, dst_stride_3d, num_reps_3d

transfer_3d idma_presets[] = {
{1,   1,  1,  1,  1,  1, 1},
{2,   8,  8,  8,  8,  8, 2},
{3,   8,  8,  8,  8,  8, 4},
{4,   8, 16, 16, 16, 16, 8},
{8,   8, 32, 32, 32, 32, 8},
{16,  8, 16, 16, 16, 16, 8},
{32,  2,  2,  2,  2,  2, 2},
{64,  4,  4,  4,  4,  4, 2},
{128, 4,  4,  4,  4,  4, 4},
};

