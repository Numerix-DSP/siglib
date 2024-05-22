# SigLib Julia Example Program
# Copyright (C) 2023 Delta Numerix

A = [3.4, 1.8, -2.8, 6.4]

B = Array{Float64,1}(undef, length(A))

absMax = ccall((:SDA_AbsMax, "siglib"), Float64, (Ptr{Cdouble},Cint), A, length(A))

println("absMax: ", absMax)

ccall((:SDA_SortMinToMax, "siglib"), Cvoid, (Ptr{Cdouble},Ptr{Cdouble},Cint), A, B, length(A))

println("B: ", B)

