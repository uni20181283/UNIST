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
	let num: Vec<&str> = line.split_whitespace().collect();
	if index==0{
		continue;
	}
	else{
		let a :i32 = num[0].trim().parse().unwrap();
		if a<10 || a>100000 {
			println!("#{} -1", index);
			write!(outfile,"#{} -1\n", index).ok();
		}
		else{
			let mut x :i32=0;
			let mut integer:Vec<i32> = Vec::new();
			if a<100 {
				integer.push(a%10);
				integer.push(a/10);
				let mut sum:i32=0;
				for s in &integer{
					sum = sum+s;
				}
				for i in &integer {
					if sum == 2*i{
						x +=1;
					}
				}
			}
			else if a <1000 {
				integer.push(a%10);
				integer.push((a/10)%10);
				integer.push(a/100);
				let mut sum:i32=0;
				for s in &integer{
					sum = sum+s;
				}
				for i in &integer {
					if sum == 2*i{
						x +=1;
					}
				}
			}
			else if a <10000 {
				integer.push(a%10);
				integer.push((a/10)%10);
				integer.push((a/100)%10);
				integer.push(a/1000);
				let mut sum:i32=0;
				for s in &integer{
					sum = sum+s;
				}
				for i in &integer {
					if sum == 2*i{
						x +=1;
					}
				}
			}
			else {
				integer.push(a%10);
				integer.push((a/10)%10);
				integer.push((a/100)%10);
				integer.push((a/1000)%10);
				integer.push(a/10000);
				let mut sum:i32=0;
				for s in &integer{
					sum = sum+s;
				}
				for i in &integer {
					if sum == 2*i{
						x +=1;
					}
				}
			}
			if x>0{
				//println!("#{} {:?}", index, integer);
				println!("#{} YES", index);
				write!(outfile, "#{} YES\n", index).ok();
			}
			else{
				println!("#{} NO", index);
				write!(outfile,"#{} NO\n", index).ok();
			}
		}
        }
   }
}
