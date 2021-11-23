pub mod error;
pub mod instruction;
pub mod processor;
pub mod state;

#[cfg(not(feature = "no-entrypoint"))]
pub mod entrypoint;

pub const PRICE_SEED: &str = "price";

solana_program::declare_id!("9onZvMzqAFzSHJrLNVWfqLRFFQ5ZCGzNXB4PBxmp6z5Y");
