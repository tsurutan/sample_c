(Main.fibonacci)
@0
D=A
@ARG
D=D+M
@13
M=D
@13
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=D-M
@TRUE0
D;JGT
@SP
A=M
M=0
@END0
0;JMP
(TRUE0)
@SP
A=M
M=-1
(END0)
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@IF_TRUE
D;JNE
@IF_FALSE
0;JMP
(IF_TRUE)
@0
D=A
@ARG
D=D+M
@13
M=D
@13
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@13
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@13
A=M-1
D=M
@THAT
M=D
@2
D=A
@13
A=M-D
D=M
@THIS
M=D
@3
D=A
@13
A=M-D
D=M
@ARG
M=D
@4
D=A
@13
A=M-D
D=M
@LCL
M=D
@5
D=A
@13
A=M-D
A=M
0;JMP
(IF_FALSE)
@0
D=A
@ARG
D=D+M
@13
M=D
@13
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=D-M
D=-D
@SP
A=M
M=D
@SP
M=M+1
@return-Main.fibonacci-1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@5
D=D+A
@SP
D=D-M
D=-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(return-Main.fibonacci-1)
@0
D=A
@ARG
D=D+M
@13
M=D
@13
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=D-M
D=-D
@SP
A=M
M=D
@SP
M=M+1
@return-Main.fibonacci-2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@5
D=D+A
@SP
D=D-M
D=-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(return-Main.fibonacci-2)
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=D+M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@13
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@13
A=M-1
D=M
@THAT
M=D
@2
D=A
@13
A=M-D
D=M
@THIS
M=D
@3
D=A
@13
A=M-D
D=M
@ARG
M=D
@4
D=A
@13
A=M-D
D=M
@LCL
M=D
@5
D=A
@13
A=M-D
A=M
0;JMP
(END)
@END
0;JMP
