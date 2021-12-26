use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_3_bonus.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_3_bonus.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.

	if index==0{
		continue;
	}
	else{
		let num: Vec<&str> = line.split_whitespace().collect();
		let a:&str = num[0];
		let b:&str = num[1];
		println!("{} {}",a,b);
		println!("{} {}",a.len(),b.len());
		let mut x:&str="l";
		if a.len()==b.len(){
			x=&(a.to_owned()+&b.to_owned());
		}
		else if a.len()>b.len(){
		}
		else {
		}

		println!("#{} {}", index, x);
		write!(outfile, "#{} {}\n", index, x).ok();
        }
   }
}
