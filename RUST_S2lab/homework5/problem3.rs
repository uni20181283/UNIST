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
    for (index , line, line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
	if index==0{
		continue;
	}
	else{
		let mut int_arr :Vec<i64> = Vec::new();
		let mut integer:i64;
		for s in num{
			integer = s.parse().unwrap();
			int_arr.push(integer);
		}
		//bubble_sort(&mut int_arr);
		if int_arr[0]<0 || int_arr[3]>1000000000{
			write!(outfile,"#{} -1\n", index).ok();
		}
		else{
			println!("{:?}",int_arr);
			println!("#{} {} {} {} {}", index, int_arr[0],int_arr[2],int_arr[1],int_arr[3]);
			write!(outfile, "#{} {} {} {} {}\n", index, int_arr[0],int_arr[2],int_arr[1],int_arr[3]).ok();
        	}
        }
   }
}
// i can't think how to read lines with 2 line
