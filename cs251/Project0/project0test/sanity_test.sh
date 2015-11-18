javac Project0.java

java Project0 < data/test1.data.txt > data/output1.data.txt

if cmp -s data/output1.data.txt data/expected_output1_data.txt
then
   echo "Sanity Test Passed!"
else
   echo "Output Differs"
fi
