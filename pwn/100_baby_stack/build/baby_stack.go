package main

import (
   "fmt"
   "os"
   "bufio"
   "unsafe"
)

func main(){
	buf := make([]byte,32)
	stdin := bufio.NewScanner(os.Stdin)

	fmt.Printf("Please tell me your name >> ");
	stdin.Scan()
	name := stdin.Text()

	fmt.Printf("Give me your message >> ");
	stdin.Scan()
	text := stdin.Text()
	memcpy(*(*uintptr)(unsafe.Pointer(&buf)), *(*uintptr)(unsafe.Pointer(&text)), len(text))

	fmt.Printf("Thank you, %s!\nmsg : %s\n", name, string(buf))
}

func memcpy(dst uintptr, src uintptr, len int){
	for i := 0; i < len; i++ {
		*(*int8)(unsafe.Pointer(dst)) = *(*int8)(unsafe.Pointer(src))
		dst += 1
		src += 1
	}
}

