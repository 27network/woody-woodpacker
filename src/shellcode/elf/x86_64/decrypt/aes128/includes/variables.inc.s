align	16
s_box		dd S_Box_row0, S_Box_row1, S_Box_row2, S_Box_row3
			dd S_Box_row4, S_Box_row5, S_Box_row6, S_Box_row7
			dd S_Box_row8, S_Box_row9, S_Box_row10, S_Box_row11
			dd S_Box_row12, S_Box_row13, S_Box_row14, S_Box_row15

align	16
inv_s_box		dd inv_S_Box_row0, inv_S_Box_row1, inv_S_Box_row2, inv_S_Box_row3
			dd inv_S_Box_row4, inv_S_Box_row5, inv_S_Box_row6, inv_S_Box_row7
			dd inv_S_Box_row8, inv_S_Box_row9, inv_S_Box_row10, inv_S_Box_row11
			dd inv_S_Box_row12, inv_S_Box_row13, inv_S_Box_row14, inv_S_Box_row15

align	16
inv_mixcolumns_matrix	db inv_mix_columns_matrix_raw0
			db inv_mix_columns_matrix_raw1
			db inv_mix_columns_matrix_raw2
			db inv_mix_columns_matrix_raw3

align   16
Rcon_table              dd 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000

align	16
vector_depacking_mask	db 0x00, 0x04, 0x08, 0x0c	; vector_depacking_mask
			db 0x80, 0x80, 0x80, 0x80
			db 0x80, 0x80, 0x80, 0x80
			db 0x80, 0x80, 0x80, 0x80

align	32
inv_shift_mask		db 0x00, 0x0d, 0x0a, 0x07	; invShiftRows_mask
			db 0x04, 0x01, 0x0e, 0x0b
			db 0x08, 0x05, 0x02, 0x0f
			db 0x0c, 0x09, 0x06, 0x03
