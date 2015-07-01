#include "stdafx.h"
#include "GCDynamicTime.h"


GCDynamicTime::GCDynamicTime(void)
{
}


GCDynamicTime::~GCDynamicTime(void)
{
}

// since 1800
double GCDynamicTime::epoch1[] = {
    // start for 1800, jan
    // 2378496.500000
        12.6, 12.3, 12, 11.9, 11.8, 11.6, 11.4, 11.2, 11.1, 11.1, 
        11.1, 11.1, 11.1, 11.2, 11.1, 11.1, 11.2, 11.4, 11.5, 11.3,
        11.2, 11.4, 11.7, 11.9, 11.9, 11.9, 11.8, 11.7, 11.8, 11.8,
        11.8, 11.7, 11.6, 11.6, 11.5, 11.5, 11.4, 11.4, 11.3, 11.3,
        11.13, 11.16, 10.94, 10.72, 10.29, 10.04, 9.94, 9.91, 9.88, 
        9.86, 9.72, 9.67, 9.66, 9.64, 9.51, 9.4, 9.21, 9, 8.6, 8.29,
        7.95, 7.73, 7.59, 7.49, 7.36, 7.26, 7.1, 7, 6.89, 6.82, 6.73,
        6.64, 6.39, 6.28, 6.25, 6.27, 6.25, 6.27, 6.22, 6.24, 6.22, 6.27,
        6.3, 6.36, 6.35, 6.37, 6.32, 6.33, 6.33, 6.37, 6.37, 6.41, 6.4,
        6.44, 6.46, 6.51, 6.48, 6.51, 6.53, 6.58, 6.55, 6.61, 6.69, 6.8,
        6.84, 6.94, 7.03, 7.13, 7.15, 7.22, 7.26, 7.3, 7.23, 7.22, 7.21,
        7.2, 6.99, 6.98, 7.19, 7.36, 7.35, 7.39, 7.41, 7.45, 7.36, 7.18,
        6.95, 6.72, 6.45, 6.24, 5.92, 5.59, 5.15, 4.67, 4.11, 3.52, 2.94,
        2.47, 1.97, 1.52, 1.04, 0.6, 0.11, -0.34, -0.82, -1.25, -1.7, -2.08,
        -2.48, -2.82, -3.19, -3.5, -3.84, -4.14, -4.43, -4.59, -4.79, -4.92, -5.09,
        -5.24, -5.36, -5.34, -5.37, -5.32, -5.34, -5.33, -5.4, -5.47, -5.58, -5.66,
        -5.74, -5.68, -5.69, -5.65, -5.67, -5.68, -5.73, -5.72, -5.78, -5.79, -5.86,
        -5.89, -6.01, -6.13, -6.28, -6.41, -6.53, -6.49, -6.5, -6.45, -6.41, -6.26,
        -6.11, -5.9, -5.63, -5.13, -4.68, -4.19, -3.72, -3.21, -2.7, -2.09, -1.48,
        -0.75, -0.08, 0.62, 1.26, 1.95, 2.59, 3.28, 3.92, 4.61, 5.2, 5.73, 6.29,
        7, 7.68, 8.45, 9.13, 9.78, 10.38, 10.99, 11.64, 12.47, 13.23, 14,
        14.69, 15.38, 16, 16.64, 17.19, 17.72, 18.19, 18.67, 19.13, 19.69, 20.14,
        20.54, 20.86, 21.14, 21.41, 21.78, 22.06, 22.3, 22.51, 22.79, 23.01, 23.29,
        23.46, 23.55, 23.63, 23.8, 23.95, 24.25, 24.39, 24.42, 24.34, 24.22, 24.1,
        24.08, 24.02, 24.04, 23.98, 23.91, 23.89, 23.95, 23.93, 23.92, 23.88, 23.94,
        23.91, 23.82, 23.76, 23.87, 23.91, 23.95, 23.96, 24, 24.04, 24.2, 24.35,
        24.61, 24.82, 25.09, 25.3, 25.56, 25.77, 26.05, 26.27, 26.54, 26.76, 27.04,
        27.27, 27.55, 27.77, 28.03, 28.25, 28.5, 28.7, 28.95, 29.15, 29.38, 29.57,
        29.8, 29.97, 30.19, 30.36, 30.57, 30.72, 30.93, 31.07, 31.24, 31.349, 31.516,
        31.677, 31.923, 32.166, 32.449, 32.671, 32.919, 33.15, 33.397, 33.584, 33.804,
        33.992, 34.24, 34.466, 34.731, 35.03, 35.4, 35.738, 36.147, 36.546, 36.995, 37.429,
        37.879, 38.291, 38.753, 39.204, 39.707, 40.182, 40.706, 41.17, 41.686, 42.227, 42.825,
        43.373, 43.959, 44.486, 44.997, 45.477, 45.983, 46.458, 46.997, 47.521, 48.034, 48.535,
        49.099, 49.589, 50.102, 50.54, 50.975, 51.382, 51.81, 52.168, 52.572, 52.957, 53.434,
        // next values starts for 1984, jan
        53.789, 54.087

};


double GCDynamicTime::epoch2[] = {
    // 1974 - 2014
    // 2442048.500000
    44.4841, 44.5646, 44.6425, 44.7386, 44.837, 44.9302, 44.9986, 45.0584, 45.1284, 45.2064, 45.298, 45.3897,
    45.4761, 45.5633, 45.645, 45.7375, 45.8284, 45.9133, 45.982, 46.0408, 46.1067, 46.1825, 46.2789, 46.3713, 
    46.4567, 46.5445, 46.6311, 46.7302, 46.8284, 46.9247, 46.997, 47.0709, 47.1451, 47.2362, 47.3413, 47.4319, 
    47.5214, 47.6049, 47.6837, 47.7781, 47.8771, 47.9687, 48.0348, 48.0942, 48.1608, 48.246, 48.3439, 48.4355,
    48.5344, 48.6325, 48.7294, 48.8365, 48.9353, 49.0319, 49.1013, 49.1591, 49.2286, 49.307, 49.4018, 49.4945,
    49.5862, 49.6805, 49.7602, 49.8556, 49.9489, 50.0347, 50.1019, 50.1622, 50.226, 50.2968, 50.3831, 50.4599,
    50.5387, 50.6161, 50.6866, 50.7658, 50.8454, 50.9187, 50.9761, 51.0278, 51.0843, 51.1538, 51.2319, 51.3063,
    51.3808, 51.4526, 51.516, 51.5985, 51.6809, 51.7573, 51.8133, 51.8532, 51.9014, 51.9603, 52.0328, 52.0985,
    52.1668, 52.2316, 52.2938, 52.368, 52.4465, 52.518, 52.5752, 52.6178, 52.6668, 52.734, 52.8056, 52.8792, 
    52.9565, 53.0445, 53.1268, 53.2197, 53.3024, 53.3747, 53.4335, 53.4778, 53.53, 53.5845, 53.6523, 53.7256,
    53.7882, 53.8367, 53.883, 53.9443, 54.0042, 54.0536, 54.0856, 54.1084, 54.1463, 54.1914, 54.2452, 54.2958,
    54.3427, 54.3911, 54.432, 54.4898, 54.5456, 54.5977, 54.6355, 54.6532, 54.6776, 54.7174, 54.7741, 54.8253,
    54.8713, 54.9161, 54.9581, 54.9997, 55.0476, 55.0912, 55.1132, 55.1328, 55.1532, 55.1898, 55.2416, 55.2838,
    55.3222, 55.3613, 55.4063, 55.4629, 55.5111, 55.5524, 55.5812, 55.6004, 55.6262, 55.6656, 55.7168, 55.7698,
    55.8197, 55.8615, 55.913, 55.9663, 56.022, 56.07, 56.0939, 56.1105, 56.1314, 56.1611, 56.2068, 56.2583,
    56.3, 56.3399, 56.379, 56.4283, 56.4804, 56.5352, 56.5697, 56.5983, 56.6328, 56.6739, 56.7332, 56.7972,
    56.8553, 56.9111, 56.9755, 57.0471, 57.1136, 57.1738, 57.2226, 57.2597, 57.3073, 57.3643, 57.4334, 57.5016,
    57.5653, 57.6333, 57.6973, 57.7711, 57.8407, 57.9058, 57.9576, 57.9975, 58.0426, 58.1043, 58.1679, 58.2389,
    58.3092, 58.3833, 58.4537, 58.5401, 58.6228, 58.6917, 58.741, 58.7836, 58.8406, 58.8986, 58.9714, 59.0438,
    59.1218, 59.2003, 59.2747, 59.3574, 59.4434, 59.5242, 59.585, 59.6344, 59.6928, 59.7588, 59.8386, 59.9111,
    59.9845, 60.0564, 60.1231, 60.2042, 60.2804, 60.353, 60.4012, 60.444, 60.49, 60.5578, 60.6324, 60.7059,
    60.7853, 60.8664, 60.9387, 61.0277, 61.1103, 61.187, 61.2454, 61.2881, 61.3378, 61.4036, 61.476, 61.5525,
    61.6287, 61.6846, 61.7433, 61.8132, 61.8823, 61.9497, 61.9969, 62.0343, 62.0714, 62.1202, 62.181, 62.2382,
    62.295, 62.3506, 62.3995, 62.4754, 62.5463, 62.6136, 62.6571, 62.6942, 62.7383, 62.7926, 62.8567, 62.9146,
    62.9659, 63.0217, 63.0807, 63.1462, 63.2053, 63.2599, 63.2844, 63.2961, 63.3126, 63.3422, 63.3871, 63.4339,
    63.4673, 63.4979, 63.5319, 63.5679, 63.6104, 63.6444, 63.6642, 63.6739, 63.6926, 63.7147, 63.7518, 63.7927,
    63.8285, 63.8557, 63.8804, 63.9075, 63.9393, 63.9691, 63.9799, 63.9833, 63.9938, 64.0093, 64.04, 64.067,
    64.0908, 64.1068, 64.1282, 64.1584, 64.1833, 64.2094, 64.2117, 64.2073, 64.2116, 64.2223, 64.25, 64.2761,
    64.2998, 64.3192, 64.345, 64.3735, 64.3943, 64.4151, 64.4132, 64.4118, 64.4097, 64.4168, 64.4329, 64.4511,
    64.4734, 64.4893, 64.5053, 64.5269, 64.5471, 64.5597, 64.5512, 64.5371, 64.5359, 64.5415, 64.5544, 64.5654,
    64.5736, 64.5891, 64.6015, 64.6176, 64.6374, 64.6549, 64.653, 64.6379, 64.6372, 64.64, 64.6543, 64.6723,
    64.6876, 64.7052, 64.7313, 64.7575, 64.7811, 64.8001, 64.7995, 64.7876, 64.7831, 64.7921, 64.8096, 64.8311,
    64.8452, 64.8597, 64.885, 64.9175, 64.948, 64.9794, 64.9895, 65.0028, 65.0138, 65.0371, 65.0773, 65.1122, 
    65.1464, 65.1833, 65.2145, 65.2494, 65.2921, 65.3279, 65.3413, 65.3452, 65.3496, 65.3711, 65.3972, 65.4296,
    65.4573, 65.4868, 65.5152, 65.545, 65.5781, 65.6127, 65.6288, 65.637, 65.6493, 65.676, 65.7097, 65.7461,
    65.7768, 65.8025, 65.8237, 65.8595, 65.8973, 65.9323, 65.9509, 65.9534, 65.9628, 65.9839, 66.0147, 66.042,
    66.0699, 66.0961, 66.131, 66.1683, 66.2072, 66.2356, 66.2409, 66.2335, 66.2349, 66.2441, 66.2751, 66.3054,
    66.3246, 66.3406, 66.3624, 66.3957, 66.4289, 66.4619, 66.4749, 66.4751, 66.4829, 66.5056, 66.5383, 66.5706,
    66.603, 66.634, 66.6569, 66.6925, 66.7289, 66.7579, 66.7708, 66.774, 66.7846, 66.8103, 66.84, 66.8779,
    66.9069, 66.9443, 66.9763, 67.0258, 67.0716, 67.11, 67.1266, 67.1331, 67.1458, 67.1717, 67.2091, 67.246, 
    // last value is 2014 jan
    67.281 
};

/// <summary>
/// Predictions for DeltaT
/// </summary>
double GCDynamicTime::epoch3[] = {
    // start year 2014, jan
    // 2456658.500000
    67.281, 67.379, 67.7,   67.8,
    67.9,   68.0,   68.1,   68.3,   68.4,   68.5,   69.0,   69.0,
    69.0,   69.0,   69.0,   69.0,   69.0,   69.0,   70.0,   70.0,
    70.0,   70.0,   70.0,   70.0,   70.0,   70.0,   71.0,   71.0,
    71.0,   71.0,   71.0,   71.0,   71.0,   71.0,   71.0,   72.0,
    72.0,   72.0,   72.0,   72.0,   72.0,   72.0,   72.0,   72.0
};

double GCDynamicTime::GetDeltaT(double julian)
{
    double * arrayp;
    int index;
	int maxidx;

    // 2014 - future
    // 2456293.500000
    // values every 3 months
    if (julian > 2456658.5)
    {
        arrayp = epoch3;
		maxidx = sizeof(epoch3)/ sizeof(double);
        index = (int)((julian - 2456658.5) * 4 / 360.25);
    }
    else if (julian > 2442048.500000)
    {
        // 1974 - 2014
        // 2442048.500000
        // values every 1 month
        arrayp = epoch2;
		maxidx = sizeof(epoch2)/ sizeof(double);
        index = (int)((julian - 2442048.500000) * 12 / 360.25);
    }
    else
    {
        // before 1800 - 1973
        // values every 6 month
        arrayp = epoch1;
		maxidx = sizeof(epoch1)/ sizeof(double);
        index = (int)((julian - 2378496.500000) * 2 / 360.25);
    }

    if (index < 0)
        index = 0;
    if (index >= maxidx)
        index = maxidx - 1;

    return arrayp[index];
}

double GCDynamicTime::getUniversalTimeFromDynamicTime(double jd)
{
    return jd - GetDeltaT(jd)/86400.0;
}

double GCDynamicTime::getDynamicTimeFromUniversalTime(double j)
{
    return j + GetDeltaT(j)/86400.0;
}
