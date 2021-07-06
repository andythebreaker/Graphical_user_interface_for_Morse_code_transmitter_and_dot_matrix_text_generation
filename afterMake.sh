echo ---afterMake.sh---
cd ~/git/Graphical_user_interface_for_Morse_code_transmitter_and_dot_matrix_text_generation
cp ~/f2/* .
value=`cat msg.txt`
git pull
git add -A
git commit -m "$value auto"
git push
echo msg_ > msg.txt
scp m2.ko em_up: