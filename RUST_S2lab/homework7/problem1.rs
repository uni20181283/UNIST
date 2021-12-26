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
	let num: Vec<&str> = line.split_whitespace().collect();
	if index==0{
		continue;
	}
	else{
		let a :i64 = num[0].trim().parse().unwrap();
		if a<1 || a>40 {
			write!(outfile,"#{} -1\n", index).ok();
			println!("#{} -1",index);
		}
		else{
			let result:u64=one(a);
			println!("#{} {}", index, result);
			write!(outfile, "#{} {}\n", index, result).ok();
        	}
        }
   }
}
fn one(a: i64) -> u64 {
	if a==1{
		return 1;
	}
	else if a==2{
		return 2;
	}
	else if a==3{
		return 4;
	}
	else if a==10{
		return 274;
	}
	else if a==11{
		return 504;
	}
	else if a==12{
		return 927;
	}
	else{
    		return one(a-1)+one(a-2)+one(a-3);
	}
}
