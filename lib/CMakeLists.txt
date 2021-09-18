cmake_minimum_required(VERSION 3.5)

add_library(
	lib
	STATIC
	symmetric_matrix.cpp
	symmetric_matrix.h
	symmetric_matrix_impl.h
	symmetric_matrix_iterator.h
	position.h
	)
	
set_target_properties(
	lib PROPERTIES
	CXX_STANDART 11
	CXX_STANDART_REQUIRED ON
)