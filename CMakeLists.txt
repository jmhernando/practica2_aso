cmake_minimum_required(VERSION 3.24)
project(practica2_mpi C)

set(CMAKE_C_STANDARD 11)

add_executable(practica2_mpi Coche.c Parking.c Camion.c CocheBueno.c CamionBueno.c ParkingBueno.c)
