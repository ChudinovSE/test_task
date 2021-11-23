use borsh::{BorshDeserialize, BorshSerialize};
use solana_program::{
    instruction::{AccountMeta, Instruction},
    pubkey::Pubkey,
    system_program, sysvar,
};

use crate::{
    id,
    state::Price,
};

#[derive(BorshSerialize, BorshDeserialize, Debug, Clone, PartialEq)]
pub enum StoreInstruction {
    /// Init store
    /// Accounts:
    /// 0. `[signer, writable]` Admin of store
    /// 1. `[writable]` Price account, PDA 
    /// 2. `[]` Rent sysvar
    /// 3. `[]` System program
    InitializeStore {price: u64},

    /// Update store
    /// Accounts:
    /// 0. `[signer, writable]` Admin of store
    /// 1. `[writable]` Price account, PDA 
    UpdatePrice {price: u64},

    Sell {amount: u64},

    /// Update store
    /// Accounts:
    /// 0. `[signer, writable]` User-payer
    /// 1. `[]` Admin of store
    /// 2. `[]` Price account, PDA 
    /// 3. `[]` System program 
    Buy {amount: u64},
}

impl StoreInstruction {
    pub fn init (
        admin: &Pubkey,
        price: u64,
    ) -> Instruction {
        let (price_pubkey, _) = Price::get_price_pubkey_with_bump();
        Instruction::new_with_borsh(
            id(),
            &StoreInstruction::InitializeStore{price},
            vec![
                AccountMeta::new(*admin, true),
                AccountMeta::new(price_pubkey, false),
                AccountMeta::new_readonly(sysvar::rent::id(), false),
                AccountMeta::new_readonly(system_program::id(), false),
            ]
        )
    }

    pub fn update (
        admin: &Pubkey,
        price: u64,
    ) -> Instruction {
        let (price_pubkey, _) = Price::get_price_pubkey_with_bump();
        Instruction::new_with_borsh(
            id(),
            &StoreInstruction::UpdatePrice{price},
            vec![
                AccountMeta::new(*admin, true),
                AccountMeta::new(price_pubkey, false),
            ]
        )
    }

    pub fn buy (
        user: &Pubkey,
        admin: &Pubkey,
        amount: u64,
    ) -> Instruction {
        let (price_pubkey, _) = Price::get_price_pubkey_with_bump();
        Instruction::new_with_borsh(
            id(),
            &StoreInstruction::Buy{amount},
            vec![
                AccountMeta::new(*user, true),
                AccountMeta::new(*admin, false),
                AccountMeta::new(price_pubkey, false),
                AccountMeta::new_readonly(system_program::id(), false)
            ]
        )
    }
}