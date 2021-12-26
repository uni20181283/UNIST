use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_1.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
	if index==0{
		continue;
	}
	else{
		let _a :i32 = num[0].trim().parse().unwrap();
		let mut int_arr :Vec<i32> = Vec::new();
		let mut integer:i32;
		for s in num{
			integer = s.parse().unwrap();
			int_arr.push(integer);

		}
		let mut x = int_arr[1];
		for i in int_arr{
			if i>x{
				x=i;
			}
		}
		println!("#{} {}", index, x);
		write!(outfile, "#{} {}\n", index, x).ok();
        }
   }
}
write!(outfile,"#{} -1\n",index).ok();
//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry//retry
