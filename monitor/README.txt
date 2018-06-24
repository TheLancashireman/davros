A serial monitor/boot loader


Commands (not case sensitive):
	Sn....	- Type n S-Record 
	Ba		- display value of byte at location a
	Ha		- display value of 16-bit word at location a
	Wa		- display value of 32-bit word at location a
	Qa		- display value of 64-bit word at location a
	Ba=v	- set byte at location a to v
	Ha=v	- set 16-bit word at location a to v
	Wa=v	- set 32-bit word at location a to v
	Qa=v	- set 64-bit word at location a to v
	Da,l,s	- dump l words memory starting at a. Word size is s.
	Ma,s	- modify memory starting at a. Word size is s.
	Ga		- call subroutine at address a
