# SigLib Perl Example Program
# Copyright (C) 2023 Delta Numerix

use siglib_wrap;

print("Testing the SigLib library SDA_Add2 function\n");

$a = siglib_wrap::new_doubleArray(3);
$b = siglib_wrap::new_doubleArray(3);
$c = siglib_wrap::new_doubleArray(3);

# Fill the arrays with some data
for ($i=0; $i<3; $i++)
{
	siglib_wrap::doubleArray_setitem($a, $i, $i+10);
	siglib_wrap::doubleArray_setitem($b, $i, $i*2);
}

for ($i=0; $i<3; $i++)
{
	print("    ", siglib_wrap::doubleArray_getitem($a, $i),"\n");
}

print("b = \n");
for ($i=0; $i<3; $i++)
{
	print("    ", siglib_wrap::doubleArray_getitem($b, $i),"\n");
}

siglib_wrap::SDA_Add2($a, $b, $c, 3);

print("c = \n");
for ($i=0; $i<3; $i++)
{
	print("    ", siglib_wrap::doubleArray_getitem($c, $i),"\n");
}

