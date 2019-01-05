use libc::{clockid_t, timespec, c_int};
use lazy_static::lazy_static;

#[no_mangle]
pub extern "C" fn clock_gettime(clk_id: clockid_t, tp: *mut timespec) -> c_int {
    type ClockGettimeType = fn(clk_id: clockid_t, tp: *mut timespec) -> c_int;
    unsafe {
        lazy_static! {
            static ref real_clock_gettime: ClockGettimeType = unsafe {
                let f = libc::dlsym(libc::RTLD_NEXT, "clock_gettime\0".as_ptr() as *const i8);
                std::mem::transmute::<*const libc::c_void, ClockGettimeType>(f)
            };
        }
        let ret = real_clock_gettime(clk_id, tp);
        if ret != -1 {
            let amount_to_slow = 4;
            let nsec_frac = 1000000000 / amount_to_slow;
            let nsec_part = nsec_frac * ((*tp).tv_sec % amount_to_slow);
            (*tp).tv_nsec = (*tp).tv_nsec / amount_to_slow + nsec_part;
            (*tp).tv_sec = (*tp).tv_sec / amount_to_slow;
        }
        ret
    }
}
