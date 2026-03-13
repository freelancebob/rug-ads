if [ $(wc -c < output.txt) -gt 1000 ]; then
  > output.txt
fi

make filesystem

echo "[[ Test at $(date) ]]" >> output.txt
valgrind --leak-check=full -- ./filesystem < input.txt >> output.txt 2>> output.txt
echo "[[ End test ]]" >> output.txt