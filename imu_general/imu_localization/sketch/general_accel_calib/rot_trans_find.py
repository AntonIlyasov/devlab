import sympy as sp
import numpy as np
from sympy import *
from sympy import symbols, cot
from math import e, sqrt
from sympy.matrices import Matrix
from sympy.plotting import plot
from sympy.plotting import plot3d
from sympy.plotting import plot3d_parametric_line

#Simpy easily allows us to solve systems of equations
x1, x2 = sp.symbols("x1 x2")

equations = [
    sp.Eq( 2*x1 + 1*x2 ,  10 ),
    sp.Eq( 1*x1 - 2*x2 ,  11 )
]

print(sp.solve(equations))