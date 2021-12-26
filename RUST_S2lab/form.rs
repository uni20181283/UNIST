use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main(){
	let _filename = "src/main.rs";
	let file = File::open("input.txt").unwrap();
	let reader = BufReader::new(file);

	
	let outfile = std::fs::File::create("output.txt").expect("create failed");
	let mut outfile = BufWriter::new(outfile);

	let mut int_arr = Vec::new();
	let mut integer:i32;	
	for (index, line) in reader.lines(){
		let split:Vec<String>= line.unwrap().split(" ").map(String::from).collect();
		for s in split {
			integer = s.parse().unwrap();
			int_arr.push(integer);
		}
		
		
		write!(outfile, "#{} {}\n", index, int_arr[1]).ok();
	}
	
}
//read line by line.

use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_2.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
	let a :i32 = num[0].trim().parse().unwrap();
	let mut x :i32 =0;
// for i in -.. a+1 is right? i think a-1 is right!
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
	if index==0{
		continue;
	}
	else{
		println!("#{} {}", index, x);
		write!(outfile, "#{} {}\n", index, x).ok();
        }
   }
}
