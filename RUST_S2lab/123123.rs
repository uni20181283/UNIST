use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{BufWriter, Write};

fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input2_homework2.txt").unwrap();
    let reader = BufReader::new(file);
    
    let f = File::create("output2_2.txt").expect("Unalbe to create file");
    let mut f = BufWriter::new(f);
    
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    for (index, line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
	let a :i32 = num[0].trim().parse().unwrap();
	let mut x :i32 =0;
// for i in -.. a+1 is right? I think a-1 is right!
	for i in 0.. a+1{
		let mut y :i32 =0;
		for j in 0..i{
			let mut z :i32 =1;
			for _k in 0..j{
				z *=10;
			}
			y+=z;
		}
		x+=y;
	}
        println!("#{} {}", index + 1, x);
        f.write("#{} {}", index + 1, x).all(data.as_bytes()).expect("Unable to write data");
   }
}
