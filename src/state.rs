use borsh::{BorshDeserialize, BorshSerialize};
use solana_program::pubkey::Pubkey;

use crate::{id, PRICE_SEED};

/// There is only one account for saving price
#[derive(BorshSerialize, BorshDeserialize, Debug)]
pub struct Price {
    /// Only admin can change this account
    pub admin: [u8; 32],
    /// Price
    pub price: u64,
}

impl Price {
    pub fn get_price_pubkey_with_bump() -> (Pubkey, u8) {
        Pubkey::find_program_address(&[PRICE_SEED.as_bytes()], &id())
    }

    pub fn get_price_pubkey() -> Pubkey {
        let (pubkey, _) = Self::get_price_pubkey_with_bump();
        pubkey
    }

    pub fn is_ok_price_pubkey(price_pubkey: &Pubkey) -> bool {
        let (pubkey, _) = Self::get_price_pubkey_with_bump();
        pubkey.to_bytes() == price_pubkey.to_bytes()
    }
}