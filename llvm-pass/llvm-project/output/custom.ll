define i32 @customFunc(){
entry:
    %x = alloca i32
    %y = alloca i32
    %z = alloca i32

    store i32 10, i32* %x
    store i32* %x, i32* %y
    %temp = load i32, i32* %y
    store i32 %temp, i32* %z

    ret i32 0
}