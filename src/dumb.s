		.data
a1:		.word	32
a2:		.word	6
		.text
main:	lw $01 a2
		lw $02 a1
		add $03 $01 $02