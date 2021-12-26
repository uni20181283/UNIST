use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_3.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_3.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.

	if index==0{
		continue;
	}
	else{
		let num: Vec<&str> = line.split_whitespace().collect();
		let a :i64 = num[0].trim().parse().unwrap();
		let b :i64 = num[1].trim().parse().unwrap();
		if a< -1 ||a>1000000000000000 || b< -1 || b>1000000000000000 {
			write!(outfile,"#{} -1\n",index).ok();
		}
		else{
		let x :i64 = a+b;
		println!("#{} {}", index, x);
		write!(outfile, "#{} {}\n", index, x).ok();
		}
        }
   }
}
