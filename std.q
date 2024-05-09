// this is going to be the standard lib, but i need the language first

func strlen(str: *u8): u32 {
  for (var i: u32 = 0;; i++) {
    if str[i] == 0 {
      return i;
    }
  }
}

// ignore this because i dont know how to do it yet
asm syscall(code: i32, arg: u8): void {
  mov rax, [esp]
  mov rdi, [esp+4]
  syscall
  leave
  ret
}

func exit(code: u8): void {
  syscall(60, code);
}