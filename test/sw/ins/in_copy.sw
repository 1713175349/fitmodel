# Test of SW potential for Si system

units		metal
boundary	p p p

atom_style	atomic
timestep 0.01
read_data	ins/data_sw
pair_style sw
pair_coeff * * ins/Si.sw Si

variable energy equal etotal

run 0
print ${energy}