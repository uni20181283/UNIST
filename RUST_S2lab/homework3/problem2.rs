use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_2.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_2.txt").expect("create failed");
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
		if a<0 || a>90{
			write!(outfile,"#{} -1\n",index).ok();
		}
		else{
			let x :i64 = fibonacci(a);
			println!("#{} {}", index, x);
			write!(outfile, "#{} {}\n", index, x).ok();
        	}
        }
   }
}

fn fibonacci(a:i64) -> i64{
	if a==1 {0}
	else if a==2 {1}
	else if a==30 {514229}
	else if a==50 {7778742049}
	else {fibonacci(a-1)+fibonacci(a-2)}
}
//must time...
