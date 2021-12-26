use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_4.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_4.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
	if index==0{
		continue;
	}
	else{
		let a :i32 = num[0].trim().parse().unwrap();

		if a<1 || a>100000000 {
			write!(outfile,"#{} -1",index).ok();
		}
		else{
			let mut result:i64=1;
			for _i in 0..a-1{
				result+=one(result);
			}
		println!("#{} {}", index, result);
		write!(outfile, "#{} {:?}\n", index, result).ok();
		}
        }
   }
}

fn one(mut a: i64) -> i64 {
	let mut x:i64=0;
	while a!=0 {
		x+=a%10;
		a = a/10;
	}
	return x
}
