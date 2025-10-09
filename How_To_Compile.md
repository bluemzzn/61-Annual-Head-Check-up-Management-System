# How to compile main program 

` gcc -o checkup main.c fortest.c unit_test.c e2e_test.c `

# Run main program
` ./checkup `

### problem
- เนื่องจากได้เพิ่มเปลี่ยนฟังก์ชันจากint age -> char age เพื่อให้ test ทำงานได้ แต่ทำให้ฟังก์ชัน main พัง สามารถรันได้แต่จะมีปัญหาที่ไฟล์ csv
- รัน unit_test or e2e_test แยกไม่ได้ เพราะเรียกฟังก์ชัน main ไปแล้วจึงเรียก main ซ้ำไม่ได้ ได้ทำการเปลี่ยนชื่อ แต่ท้ายที่สุด ก็ทำให้รันแยกกันไม่ได้ รันรวมกันได้อย่างเดียว