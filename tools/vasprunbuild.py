import ase.io as aseio
import os
import sys
import numpy as np
from ase import Atoms, Atom

def vaspruntolammps(fn:str):

    ats = aseio.read(fn, format='vasp-xml')
    print(ats.get_total_energy())


vaspruntolammps("/home/zln/tmp/vasprun.xml")