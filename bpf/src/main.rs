// SPDX-License-Identifier: GPL-2.0
mod bpf;
use bpf::*;

use libbpf_rs::skel::*;
use std::mem::MaybeUninit;

use std::sync::Arc;
use std::sync::atomic::AtomicBool;
use std::sync::atomic::Ordering;

fn main() {
    let mut open_object = MaybeUninit::uninit();
    let skel_builder = ExampleSkelBuilder::default();
    let open_skel = skel_builder.open(&mut open_object).unwrap();
    let mut skel = open_skel.load().unwrap();
    let _link = skel.maps.my_ops_sample.attach_struct_ops().unwrap();
    println!("Successfully attached bpf program!");

    // Register Ctrl+C handler that terminate this app
    let shutdown = Arc::new(AtomicBool::new(false));
    let shutdown_clone = shutdown.clone();
    ctrlc::set_handler(move || {
        println!("Ctrl+C is sent!");
        shutdown_clone.store(true, Ordering::Relaxed);
    }).expect("Error setting Ctrl+C handler");

    while !shutdown.load(Ordering::Relaxed) {
        let duration = std::time::Duration::from_millis(100);
        std::thread::sleep(duration);
    }
    println!("Shutdown..");
}
