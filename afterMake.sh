echo ---afterMake.sh---
cd ~/git/Graphical_user_interface_for_Morse_code_transmitter_and_dot_matrix_text_generation
cp ~/f2/* .
git add -A
git commit -m "auto"
git push
scp m2.ko em_up: