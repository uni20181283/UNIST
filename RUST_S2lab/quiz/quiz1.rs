use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_1.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_1.txt").expect("create failed");
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
		println!("a:{} b:{}",a,b);
		if a<1 || a>10 || b<1 || b>10 {
			write!(outfile,"#{} -1",index).ok();
		}
		else{
			let mut x :i64=1;
			for _i in 0..b{
				x=x*a;
			}
			println!("#{} {}", index, x);
			write!(outfile, "#{} {}\n", index, x).ok();
		}
        }
   }
}
