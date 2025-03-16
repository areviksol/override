# override

sudo qemu-system-x86_64 \
  -smp 2 \
  -cdrom /Users/arevikmkrtchyan/Downloads/OverRide.iso \
  -boot d \
  -vga std \
  -m 2G \
  -nic user,hostfwd=tcp::4242-:4242 \
  -display default,show-cursor=on


ssh -p 4242 level00@127.0.0.1