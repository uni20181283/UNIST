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
		let b :i64 = num[1].trim().parse().unwrap();
		println!("a:{} b:{}",a,b);
		if a<1 || a>1000 || b<10 || b>100000 {
			write!(outfile,"#{} -1\n", index).ok();
			println!("#{} -1", index);
		}
		else{
			let mut k:i64=1;
			let mut prime:i64=0;
			let q :i64 = k*k+a;
			let w :i64 = (k+1)*(k+1)+a;
			while w<b {
				for x in 1..q/2 {
					if q%x==0 && w%x==0{
						prime =x;
					}
				}
				k+=1;
			}
			println!("#{} {}", index, prime);
			write!(outfile, "#{} {}\n", index, prime).ok();
        	}
        }
   }
}
