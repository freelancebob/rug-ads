  > output.txt
make filesystem

echo "[[ Test at $(date) ]]" >> output.txt
./filesystem < input.txt >> output.txt 2>> output.txt
echo "[[ End test ]]" >> output.txt