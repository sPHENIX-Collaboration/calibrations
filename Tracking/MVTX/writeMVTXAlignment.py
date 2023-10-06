'''
Parser to get MVTX alignment information
Cameron Dean, MIT, October 2023

You may need to install a dependency with pip:
pip install xlrd
pip install scikit-spatial

Note: for the alignment you need to move the tracker hit set object which is a single chip
You must rotate the chip in local coordinates then translate the position in global coordinates 
'''

import numpy as np
import os, fnmatch
from datetime import date
from skspatial.objects import Line
import pandas as pd
pd.set_option('io.excel.xlsx.reader', "openpyxl")

verbosity = 0

mvtxAlignmentList = [
{"name": "P204", "layer": 0, "stave": 0, "chip": 0, "hitsetkey": 16, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 1, "hitsetkey": 48, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 2, "hitsetkey": 80, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 3, "hitsetkey": 112, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 4, "hitsetkey": 144, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 5, "hitsetkey": 176, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 6, "hitsetkey": 208, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 7, "hitsetkey": 240, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "P204", "layer": 0, "stave": 0, "chip": 8, "hitsetkey": 272, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 0, "hitsetkey": 528, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 1, "hitsetkey": 560, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 2, "hitsetkey": 592, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 3, "hitsetkey": 624, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 4, "hitsetkey": 656, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 5, "hitsetkey": 688, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 6, "hitsetkey": 720, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 7, "hitsetkey": 752, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A305", "layer": 0, "stave": 1, "chip": 8, "hitsetkey": 784, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 0, "hitsetkey": 1040, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 1, "hitsetkey": 1072, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 2, "hitsetkey": 1104, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 3, "hitsetkey": 1136, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 4, "hitsetkey": 1168, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 5, "hitsetkey": 1200, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 6, "hitsetkey": 1232, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 7, "hitsetkey": 1264, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K202", "layer": 0, "stave": 2, "chip": 8, "hitsetkey": 1296, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 0, "hitsetkey": 1552, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 1, "hitsetkey": 1584, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 2, "hitsetkey": 1616, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 3, "hitsetkey": 1648, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 4, "hitsetkey": 1680, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 5, "hitsetkey": 1712, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 6, "hitsetkey": 1744, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 7, "hitsetkey": 1776, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S205", "layer": 0, "stave": 3, "chip": 8, "hitsetkey": 1808, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 0, "hitsetkey": 2064, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 1, "hitsetkey": 2096, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 2, "hitsetkey": 2128, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 3, "hitsetkey": 2160, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 4, "hitsetkey": 2192, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 5, "hitsetkey": 2224, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 6, "hitsetkey": 2256, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 7, "hitsetkey": 2288, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B201", "layer": 0, "stave": 4, "chip": 8, "hitsetkey": 2320, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 0, "hitsetkey": 2576, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 1, "hitsetkey": 2608, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 2, "hitsetkey": 2640, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 3, "hitsetkey": 2672, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 4, "hitsetkey": 2704, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 5, "hitsetkey": 2736, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 6, "hitsetkey": 2768, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 7, "hitsetkey": 2800, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O204", "layer": 0, "stave": 5, "chip": 8, "hitsetkey": 2832, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 0, "hitsetkey": 3088, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 1, "hitsetkey": 3120, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 2, "hitsetkey": 3152, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 3, "hitsetkey": 3184, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 4, "hitsetkey": 3216, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 5, "hitsetkey": 3248, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 6, "hitsetkey": 3280, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 7, "hitsetkey": 3312, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L201", "layer": 0, "stave": 6, "chip": 8, "hitsetkey": 3344, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 0, "hitsetkey": 3600, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 1, "hitsetkey": 3632, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 2, "hitsetkey": 3664, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 3, "hitsetkey": 3696, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 4, "hitsetkey": 3728, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 5, "hitsetkey": 3760, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 6, "hitsetkey": 3792, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 7, "hitsetkey": 3824, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A201", "layer": 0, "stave": 7, "chip": 8, "hitsetkey": 3856, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 0, "hitsetkey": 4112, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 1, "hitsetkey": 4144, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 2, "hitsetkey": 4176, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 3, "hitsetkey": 4208, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 4, "hitsetkey": 4240, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 5, "hitsetkey": 4272, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 6, "hitsetkey": 4304, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 7, "hitsetkey": 4336, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K206", "layer": 0, "stave": 8, "chip": 8, "hitsetkey": 4368, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 0, "hitsetkey": 4624, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 1, "hitsetkey": 4656, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 2, "hitsetkey": 4688, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 3, "hitsetkey": 4720, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 4, "hitsetkey": 4752, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 5, "hitsetkey": 4784, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 6, "hitsetkey": 4816, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 7, "hitsetkey": 4848, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K204", "layer": 0, "stave": 9, "chip": 8, "hitsetkey": 4880, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 0, "hitsetkey": 5136, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 1, "hitsetkey": 5168, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 2, "hitsetkey": 5200, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 3, "hitsetkey": 5232, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 4, "hitsetkey": 5264, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 5, "hitsetkey": 5296, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 6, "hitsetkey": 5328, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 7, "hitsetkey": 5360, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A301", "layer": 0, "stave": 10, "chip": 8, "hitsetkey": 5392, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 0, "hitsetkey": 5648, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 1, "hitsetkey": 5680, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 2, "hitsetkey": 5712, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 3, "hitsetkey": 5744, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 4, "hitsetkey": 5776, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 5, "hitsetkey": 5808, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 6, "hitsetkey": 5840, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 7, "hitsetkey": 5872, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K205", "layer": 0, "stave": 11, "chip": 8, "hitsetkey": 5904, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 0, "hitsetkey": 65552, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 1, "hitsetkey": 65584, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 2, "hitsetkey": 65616, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 3, "hitsetkey": 65648, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 4, "hitsetkey": 65680, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 5, "hitsetkey": 65712, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 6, "hitsetkey": 65744, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 7, "hitsetkey": 65776, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I205", "layer": 1, "stave": 0, "chip": 8, "hitsetkey": 65808, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 0, "hitsetkey": 66064, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 1, "hitsetkey": 66096, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 2, "hitsetkey": 66128, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 3, "hitsetkey": 66160, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 4, "hitsetkey": 66192, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 5, "hitsetkey": 66224, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 6, "hitsetkey": 66256, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 7, "hitsetkey": 66288, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T201", "layer": 1, "stave": 1, "chip": 8, "hitsetkey": 66320, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 0, "hitsetkey": 66576, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 1, "hitsetkey": 66608, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 2, "hitsetkey": 66640, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 3, "hitsetkey": 66672, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 4, "hitsetkey": 66704, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 5, "hitsetkey": 66736, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 6, "hitsetkey": 66768, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 7, "hitsetkey": 66800, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X205", "layer": 1, "stave": 2, "chip": 8, "hitsetkey": 66832, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 0, "hitsetkey": 67088, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 1, "hitsetkey": 67120, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 2, "hitsetkey": 67152, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 3, "hitsetkey": 67184, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 4, "hitsetkey": 67216, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 5, "hitsetkey": 67248, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 6, "hitsetkey": 67280, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 7, "hitsetkey": 67312, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T205", "layer": 1, "stave": 3, "chip": 8, "hitsetkey": 67344, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 0, "hitsetkey": 67600, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 1, "hitsetkey": 67632, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 2, "hitsetkey": 67664, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 3, "hitsetkey": 67696, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 4, "hitsetkey": 67728, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 5, "hitsetkey": 67760, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 6, "hitsetkey": 67792, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 7, "hitsetkey": 67824, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A304", "layer": 1, "stave": 4, "chip": 8, "hitsetkey": 67856, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 0, "hitsetkey": 68112, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 1, "hitsetkey": 68144, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 2, "hitsetkey": 68176, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 3, "hitsetkey": 68208, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 4, "hitsetkey": 68240, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 5, "hitsetkey": 68272, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 6, "hitsetkey": 68304, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 7, "hitsetkey": 68336, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L202", "layer": 1, "stave": 5, "chip": 8, "hitsetkey": 68368, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 0, "hitsetkey": 68624, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 1, "hitsetkey": 68656, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 2, "hitsetkey": 68688, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 3, "hitsetkey": 68720, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 4, "hitsetkey": 68752, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 5, "hitsetkey": 68784, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 6, "hitsetkey": 68816, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 7, "hitsetkey": 68848, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "Q202", "layer": 1, "stave": 6, "chip": 8, "hitsetkey": 68880, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 0, "hitsetkey": 69136, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 1, "hitsetkey": 69168, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 2, "hitsetkey": 69200, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 3, "hitsetkey": 69232, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 4, "hitsetkey": 69264, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 5, "hitsetkey": 69296, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 6, "hitsetkey": 69328, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 7, "hitsetkey": 69360, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O201", "layer": 1, "stave": 7, "chip": 8, "hitsetkey": 69392, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 0, "hitsetkey": 69648, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 1, "hitsetkey": 69680, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 2, "hitsetkey": 69712, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 3, "hitsetkey": 69744, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 4, "hitsetkey": 69776, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 5, "hitsetkey": 69808, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 6, "hitsetkey": 69840, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 7, "hitsetkey": 69872, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R202", "layer": 1, "stave": 8, "chip": 8, "hitsetkey": 69904, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 0, "hitsetkey": 70160, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 1, "hitsetkey": 70192, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 2, "hitsetkey": 70224, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 3, "hitsetkey": 70256, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 4, "hitsetkey": 70288, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 5, "hitsetkey": 70320, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 6, "hitsetkey": 70352, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 7, "hitsetkey": 70384, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B206", "layer": 1, "stave": 9, "chip": 8, "hitsetkey": 70416, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 0, "hitsetkey": 70672, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 1, "hitsetkey": 70704, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 2, "hitsetkey": 70736, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 3, "hitsetkey": 70768, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 4, "hitsetkey": 70800, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 5, "hitsetkey": 70832, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 6, "hitsetkey": 70864, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 7, "hitsetkey": 70896, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O205", "layer": 1, "stave": 10, "chip": 8, "hitsetkey": 70928, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 0, "hitsetkey": 71184, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 1, "hitsetkey": 71216, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 2, "hitsetkey": 71248, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 3, "hitsetkey": 71280, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 4, "hitsetkey": 71312, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 5, "hitsetkey": 71344, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 6, "hitsetkey": 71376, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 7, "hitsetkey": 71408, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J202", "layer": 1, "stave": 11, "chip": 8, "hitsetkey": 71440, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 0, "hitsetkey": 71696, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 1, "hitsetkey": 71728, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 2, "hitsetkey": 71760, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 3, "hitsetkey": 71792, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 4, "hitsetkey": 71824, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 5, "hitsetkey": 71856, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 6, "hitsetkey": 71888, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 7, "hitsetkey": 71920, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X202", "layer": 1, "stave": 12, "chip": 8, "hitsetkey": 71952, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 0, "hitsetkey": 72208, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 1, "hitsetkey": 72240, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 2, "hitsetkey": 72272, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 3, "hitsetkey": 72304, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 4, "hitsetkey": 72336, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 5, "hitsetkey": 72368, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 6, "hitsetkey": 72400, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 7, "hitsetkey": 72432, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H203", "layer": 1, "stave": 13, "chip": 8, "hitsetkey": 72464, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 0, "hitsetkey": 72720, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 1, "hitsetkey": 72752, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 2, "hitsetkey": 72784, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 3, "hitsetkey": 72816, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 4, "hitsetkey": 72848, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 5, "hitsetkey": 72880, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 6, "hitsetkey": 72912, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 7, "hitsetkey": 72944, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A306", "layer": 1, "stave": 14, "chip": 8, "hitsetkey": 72976, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 0, "hitsetkey": 73232, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 1, "hitsetkey": 73264, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 2, "hitsetkey": 73296, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 3, "hitsetkey": 73328, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 4, "hitsetkey": 73360, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 5, "hitsetkey": 73392, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 6, "hitsetkey": 73424, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 7, "hitsetkey": 73456, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "G202", "layer": 1, "stave": 15, "chip": 8, "hitsetkey": 73488, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 0, "hitsetkey": 131088, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 1, "hitsetkey": 131120, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 2, "hitsetkey": 131152, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 3, "hitsetkey": 131184, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 4, "hitsetkey": 131216, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 5, "hitsetkey": 131248, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 6, "hitsetkey": 131280, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 7, "hitsetkey": 131312, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "X201", "layer": 2, "stave": 0, "chip": 8, "hitsetkey": 131344, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 0, "hitsetkey": 131600, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 1, "hitsetkey": 131632, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 2, "hitsetkey": 131664, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 3, "hitsetkey": 131696, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 4, "hitsetkey": 131728, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 5, "hitsetkey": 131760, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 6, "hitsetkey": 131792, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 7, "hitsetkey": 131824, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S201", "layer": 2, "stave": 1, "chip": 8, "hitsetkey": 131856, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 0, "hitsetkey": 132112, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 1, "hitsetkey": 132144, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 2, "hitsetkey": 132176, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 3, "hitsetkey": 132208, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 4, "hitsetkey": 132240, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 5, "hitsetkey": 132272, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 6, "hitsetkey": 132304, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 7, "hitsetkey": 132336, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "S206", "layer": 2, "stave": 2, "chip": 8, "hitsetkey": 132368, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 0, "hitsetkey": 132624, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 1, "hitsetkey": 132656, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 2, "hitsetkey": 132688, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 3, "hitsetkey": 132720, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 4, "hitsetkey": 132752, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 5, "hitsetkey": 132784, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 6, "hitsetkey": 132816, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 7, "hitsetkey": 132848, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H204", "layer": 2, "stave": 3, "chip": 8, "hitsetkey": 132880, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 0, "hitsetkey": 133136, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 1, "hitsetkey": 133168, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 2, "hitsetkey": 133200, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 3, "hitsetkey": 133232, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 4, "hitsetkey": 133264, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 5, "hitsetkey": 133296, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 6, "hitsetkey": 133328, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 7, "hitsetkey": 133360, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "O206", "layer": 2, "stave": 4, "chip": 8, "hitsetkey": 133392, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 0, "hitsetkey": 133648, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 1, "hitsetkey": 133680, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 2, "hitsetkey": 133712, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 3, "hitsetkey": 133744, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 4, "hitsetkey": 133776, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 5, "hitsetkey": 133808, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 6, "hitsetkey": 133840, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 7, "hitsetkey": 133872, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "K203", "layer": 2, "stave": 5, "chip": 8, "hitsetkey": 133904, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 0, "hitsetkey": 134160, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 1, "hitsetkey": 134192, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 2, "hitsetkey": 134224, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 3, "hitsetkey": 134256, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 4, "hitsetkey": 134288, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 5, "hitsetkey": 134320, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 6, "hitsetkey": 134352, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 7, "hitsetkey": 134384, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "H202", "layer": 2, "stave": 6, "chip": 8, "hitsetkey": 134416, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 0, "hitsetkey": 134672, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 1, "hitsetkey": 134704, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 2, "hitsetkey": 134736, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 3, "hitsetkey": 134768, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 4, "hitsetkey": 134800, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 5, "hitsetkey": 134832, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 6, "hitsetkey": 134864, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 7, "hitsetkey": 134896, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U202", "layer": 2, "stave": 7, "chip": 8, "hitsetkey": 134928, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 0, "hitsetkey": 135184, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 1, "hitsetkey": 135216, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 2, "hitsetkey": 135248, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 3, "hitsetkey": 135280, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 4, "hitsetkey": 135312, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 5, "hitsetkey": 135344, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 6, "hitsetkey": 135376, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 7, "hitsetkey": 135408, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "J204", "layer": 2, "stave": 8, "chip": 8, "hitsetkey": 135440, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 0, "hitsetkey": 135696, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 1, "hitsetkey": 135728, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 2, "hitsetkey": 135760, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 3, "hitsetkey": 135792, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 4, "hitsetkey": 135824, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 5, "hitsetkey": 135856, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 6, "hitsetkey": 135888, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 7, "hitsetkey": 135920, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "R201", "layer": 2, "stave": 9, "chip": 8, "hitsetkey": 135952, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 0, "hitsetkey": 136208, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 1, "hitsetkey": 136240, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 2, "hitsetkey": 136272, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 3, "hitsetkey": 136304, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 4, "hitsetkey": 136336, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 5, "hitsetkey": 136368, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 6, "hitsetkey": 136400, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 7, "hitsetkey": 136432, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "T202", "layer": 2, "stave": 10, "chip": 8, "hitsetkey": 136464, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 0, "hitsetkey": 136720, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 1, "hitsetkey": 136752, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 2, "hitsetkey": 136784, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 3, "hitsetkey": 136816, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 4, "hitsetkey": 136848, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 5, "hitsetkey": 136880, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 6, "hitsetkey": 136912, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 7, "hitsetkey": 136944, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A303", "layer": 2, "stave": 11, "chip": 8, "hitsetkey": 136976, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 0, "hitsetkey": 137232, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 1, "hitsetkey": 137264, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 2, "hitsetkey": 137296, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 3, "hitsetkey": 137328, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 4, "hitsetkey": 137360, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 5, "hitsetkey": 137392, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 6, "hitsetkey": 137424, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 7, "hitsetkey": 137456, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "L204", "layer": 2, "stave": 12, "chip": 8, "hitsetkey": 137488, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 0, "hitsetkey": 137744, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 1, "hitsetkey": 137776, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 2, "hitsetkey": 137808, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 3, "hitsetkey": 137840, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 4, "hitsetkey": 137872, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 5, "hitsetkey": 137904, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 6, "hitsetkey": 137936, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 7, "hitsetkey": 137968, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U206", "layer": 2, "stave": 13, "chip": 8, "hitsetkey": 138000, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 0, "hitsetkey": 138256, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 1, "hitsetkey": 138288, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 2, "hitsetkey": 138320, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 3, "hitsetkey": 138352, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 4, "hitsetkey": 138384, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 5, "hitsetkey": 138416, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 6, "hitsetkey": 138448, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 7, "hitsetkey": 138480, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "B205", "layer": 2, "stave": 14, "chip": 8, "hitsetkey": 138512, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 0, "hitsetkey": 138768, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 1, "hitsetkey": 138800, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 2, "hitsetkey": 138832, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 3, "hitsetkey": 138864, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 4, "hitsetkey": 138896, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 5, "hitsetkey": 138928, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 6, "hitsetkey": 138960, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 7, "hitsetkey": 138992, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "I202", "layer": 2, "stave": 15, "chip": 8, "hitsetkey": 139024, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 0, "hitsetkey": 139280, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 1, "hitsetkey": 139312, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 2, "hitsetkey": 139344, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 3, "hitsetkey": 139376, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 4, "hitsetkey": 139408, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 5, "hitsetkey": 139440, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 6, "hitsetkey": 139472, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 7, "hitsetkey": 139504, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A206", "layer": 2, "stave": 16, "chip": 8, "hitsetkey": 139536, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 0, "hitsetkey": 139792, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 1, "hitsetkey": 139824, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 2, "hitsetkey": 139856, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 3, "hitsetkey": 139888, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 4, "hitsetkey": 139920, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 5, "hitsetkey": 139952, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 6, "hitsetkey": 139984, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 7, "hitsetkey": 140016, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "A204", "layer": 2, "stave": 17, "chip": 8, "hitsetkey": 140048, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 0, "hitsetkey": 140304, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 1, "hitsetkey": 140336, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 2, "hitsetkey": 140368, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 3, "hitsetkey": 140400, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 4, "hitsetkey": 140432, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 5, "hitsetkey": 140464, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 6, "hitsetkey": 140496, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 7, "hitsetkey": 140528, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "M205", "layer": 2, "stave": 18, "chip": 8, "hitsetkey": 140560, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 0, "hitsetkey": 140816, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 1, "hitsetkey": 140848, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 2, "hitsetkey": 140880, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 3, "hitsetkey": 140912, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 4, "hitsetkey": 140944, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 5, "hitsetkey": 140976, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 6, "hitsetkey": 141008, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 7, "hitsetkey": 141040, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.},
{"name": "U203", "layer": 2, "stave": 19, "chip": 8, "hitsetkey": 141072, "alpha": 0., "beta": 0., "gamma": 0., "dx": 0., "dy": 0., "dz": 0.}
]

#Some staves are missing measurement points for the chip deviations
missingChipMeasurementPoints = [{"name": "B201", "missing": {35}}, 
				{"name": "K205", "missing": {19, 22}}, 
				{"name": "A201", "missing": {35}},
				{"name": "I205", "missing": {35}},
				{"name": "G202", "missing": {35}},
				{"name": "H203", "missing": {35}},
				{"name": "B206", "missing": {35}},
				{"name": "H202", "missing": {35}},
				{"name": "H204", "missing": {35}},
				{"name": "A204", "missing": {35}},
				{"name": "A206", "missing": {35}},
				{"name": "I202", "missing": {35}},
				{"name": "B205", "missing": {35}}]

today = date.today()
computerdate = today.strftime("%Y%m%d")
alignmentDir = '/sphenix/user/cdean/software/calibrations-1/Tracking/MVTX'

mvtxLayerInfo = [{"nStaves" : 12, "phi0": 0.28500, "phistep": 0.52360, "phioffset": 1.57080, "phitilt": 0.23255},
                 {"nStaves" : 16, "phi0": 0.19900, "phistep": 0.39270, "phioffset": 1.57080, "phitilt": 0.29539},
                 {"nStaves" : 20, "phi0": 0.16600, "phistep": 0.31416, "phioffset": 1.57080, "phitilt": 0.29731}]


coordinate = ["X", "Y", "Z"]
angle = ["alpha", "beta", "gamma"]
chipPositions = [{"corner": 0, coordinate[0] : -1.485, coordinate[1] :  0.74, coordinate[2] : 0.},
    		 {"corner": 1, coordinate[0] : -1.485, coordinate[1] : -0.74, coordinate[2] : 0.},
    		 {"corner": 2, coordinate[0] :  1.485, coordinate[1] : -0.74, coordinate[2] : 0.},
    		 {"corner": 3, coordinate[0] :  1.485, coordinate[1] :  0.74, coordinate[2] : 0.}]

def getLocalChipCorners( cornerDeltas ):
  localCorners = []
  for i in range(len(chipPositions)):
    corner = {key: chipPositions[i][key] + cornerDeltas[i].get(key, 0) for key in chipPositions[i].keys()}
    corner["corner"] = i
    localCorners.append(corner)
  return localCorners 

def calculteMidPoint( cornerA, cornerB ):
  midPoint = {coordinate[0] : 0., coordinate[1] : 0., coordinate[2] : 0.}
  for coord in coordinate:
    midPoint[coord] = (cornerA[coord] + cornerB[coord])/2.
  return midPoint

def calculateCenter( midPoint ):
  line1 = Line.from_points([midPoint[0]["X"], midPoint[0]["Y"], midPoint[0]["Z"]], [midPoint[2]["X"], midPoint[2]["Y"], midPoint[2]["Z"]])
  line2 = Line.from_points([midPoint[1]["X"], midPoint[1]["Y"], midPoint[1]["Z"]], [midPoint[3]["X"], midPoint[3]["Y"], midPoint[3]["Z"]])
  intersection = line1.intersect_line(line2)
  return intersection

def calculateChipRotations( chip, midPoint, chipCenter ):
  #Note, I rotate basis from metrology file to local geant here
  #check nores later 
  deltaX = midPoint[3]["Y"] - chipCenter[1] 
  deltaY = midPoint[3]["Z"] - chipCenter[2] 
  deltaZ = midPoint[2]["X"] - chipCenter[0] 
  chip["alpha"] = round(np.arctan((deltaY)/(deltaX)), 7)
  #chip["beta"] = round(np.arctan((deltaX)/(deltaZ)), 7)
  #chip["gamma"] = round(np.arctan((deltaZ)/(deltaY)), 7)

def rotateChipLocalToGlobal( chip, chipCenter ):
   layerInfo = mvtxLayerInfo[chip["layer"]]
   angle = layerInfo["phi0"] + chip["stave"]*layerInfo["phistep"] + layerInfo["phioffset"] + layerInfo["phitilt"]
   chip["dx"] = round(chipCenter[1]*np.cos(angle) - chipCenter[2]*np.sin(angle), 7)
   chip["dy"] = round(chipCenter[2]*np.sin(angle) + chipCenter[1]*np.cos(angle), 7)
   chip["dz"] = round(chipCenter[0], 7)

def find(pattern, path):
    result = []
    for root, dirs, files in os.walk(path):
        for name in files:
            if fnmatch.fnmatch(name, pattern):
                result.append(os.path.join(root, name))
    return result

def calcualteChipAlignment( staveInformation ):
  metrologyFile = find('*' + staveInformation["name"] + '*', alignmentDir + "/staves")
  numberOfMetrologyFiles = len(metrologyFile)

  if (numberOfMetrologyFiles == 0): print("There are no metrology files for stave {}".format(staveInformation["name"]))
  elif (numberOfMetrologyFiles > 1): print("There are {0} metrology files for stave {1}, I don't know which one to read".format(numberOfMetrologyFiles, staveInformation["name"]))
  else:
    if (verbosity > 0): print("Stave metrology file: {}".format(metrologyFile[0]))

    metrologyData = pd.read_excel(metrologyFile[0], sheet_name='Report', skiprows=11)
    for chip in range(0, 9):
      getChipDict = next((item for item in mvtxAlignmentList if item["layer"] == staveInformation["layer"] 
						     	    and item["stave"] == staveInformation["stave"] 
						     	    and item["chip"] == chip), None)

      cornerDeviations = [{"corner": 0, coordinate[0] : 0., coordinate[1] : 0., coordinate[2] : 0.},
          		  {"corner": 1, coordinate[0] : 0., coordinate[1] : 0., coordinate[2] : 0.},
          		  {"corner": 2, coordinate[0] : 0., coordinate[1] : 0., coordinate[2] : 0.},
          		  {"corner": 3, coordinate[0] : 0., coordinate[1] : 0., coordinate[2] : 0.}]

      i = 1
      for deviation in cornerDeviations:
        for coord in coordinate:
          measurementPoint = i + 4*chip
          missingInfo = next((item for item in missingChipMeasurementPoints if item["name"] == staveInformation["name"]), None)
          if (measurementPoint == 36): break #There are no metrology values for the final corner, need a break or we'll crash
          if (missingInfo and measurementPoint in missingInfo["missing"]): continue
          else:
            deviation[coord] = 0.1*metrologyData.iloc[metrologyData.where(metrologyData=='{0}-Valeur_P{1}'.format(coord, measurementPoint)).dropna(how='all').dropna(axis=1).index.values[0], 5]
        i+=1

      localChipCorners = getLocalChipCorners(cornerDeviations)
      midPoints = []
      for j in range(0, 4):
        nextPoint = j + 1 if j < 3 else 0
        midPoints.append(calculteMidPoint(localChipCorners[j], localChipCorners[nextPoint]))
      myCenter = calculateCenter(midPoints)
      calculateChipRotations( getChipDict, midPoints, myCenter)
      rotateChipLocalToGlobal( getChipDict, myCenter )
      if (verbosity > 0):
        print(getChipDict)

def calcualteStaveAlignment():
  print("There is no stave alignment code yet")

def calculateLayerAlignment():
  print("There is no layer alignment code yet")

def calculateDetectorAlignment():
  print("There is no detector alignment code yet")

def writeMVTXAlignment():
  alignmentFileName = "{0}/mvtxAlignmentParamsFile-{1}.txt".format(alignmentDir, computerdate)
  alignmentFile = open("{}".format(alignmentFileName), "w")
  for i in range(0, len(mvtxLayerInfo)):
   for j in range(0, mvtxLayerInfo[i]["nStaves"]):
     stave = next((item for item in mvtxAlignmentList if item["layer"] == i and item["stave"] == j), None)
     calcualteChipAlignment(stave)
  for entry in mvtxAlignmentList:
    alignmentFile.write("{0} {1} {2} {3} {4} {5} {6}\n".format(entry["hitsetkey"], entry["alpha"], entry["beta"], entry["gamma"], entry["dx"], entry["dy"], entry["dz"])) 

writeMVTXAlignment()
