use borsh::{BorshDeserialize, BorshSerialize};
use solana_program::account_info::{next_account_info, AccountInfo};
use solana_program::entrypoint::ProgramResult;
use solana_program::program::{invoke, invoke_signed};
use solana_program::program_error::ProgramError;
use solana_program::pubkey::Pubkey;
use solana_program::sysvar::{rent::Rent, Sysvar};
use solana_program::{msg, system_instruction};

use crate::{id, PRICE_SEED};
use crate::state::Price;
use crate::error::ProcessingError;
use crate::instruction::StoreInstruction;

pub struct Processor;

impl Processor {
    pub fn process(_program_id: &Pubkey, accounts: &[AccountInfo], input: &[u8]) -> ProgramResult {
        let instruction = StoreInstruction::try_from_slice(input)?;
        match instruction {
            StoreInstruction::InitializeStore { price } => init_store(accounts, price),
            StoreInstruction::UpdatePrice { price } => update_price(accounts, price),
            StoreInstruction::Sell { amount } => todo!(),
            StoreInstruction::Buy { amount } => buy_token(accounts, amount),
        }
        }
}

pub fn init_store (accounts: &[AccountInfo], start_price: u64) -> ProgramResult {

    // Checking accounts
    let acc_iter = &mut accounts.iter();
    let admin_info = next_account_info(acc_iter)?;
    let price_info = next_account_info(acc_iter)?;
    let rent_info = next_account_info(acc_iter)?;
    let system_program_info = next_account_info(acc_iter)?;

    let (price_pubkey, bump_seed) = Price::get_price_pubkey_with_bump();
    if price_pubkey != *price_info.key {
        return Err(ProgramError::InvalidArgument);
    }

    if !admin_info.is_signer {
        return Err(ProgramError::MissingRequiredSignature);
    }

    if price_info.data_is_empty() {
        let price = Price { admin: admin_info.key.to_bytes(), price: start_price};
        let space = price.try_to_vec()?.len();
        let rent = &Rent::from_account_info(rent_info)?;
        let lamports = rent.minimum_balance(space);
        let signer_seeds: &[&[_]] = &[PRICE_SEED.as_bytes(), &[bump_seed]];
        invoke_signed(
            &system_instruction::create_account(
                admin_info.key,
                &price_pubkey,
                lamports,
                space as u64,
                &id(),
            ),
            &[admin_info.clone(), price_info.clone(), system_program_info.clone()],
            &[&signer_seeds],
        )?;
    }

    else {
        return Err(ProcessingError::StoreIsInit.into());
    }

    let mut price = Price::try_from_slice(&price_info.data.borrow())?;
    if price.admin != admin_info.key.to_bytes() && price.admin != [0; 32] {
        return Err(ProcessingError::AccesDenited.into());
    }

    price.admin = admin_info.key.to_bytes();
    price.price = start_price;
    
    let _ = price.serialize(&mut &mut price_info.data.borrow_mut()[..]);
    msg!("Store is init!");
    Ok(())
}

pub fn update_price (accounts: &[AccountInfo], start_price: u64) -> ProgramResult {

    // Checking accounts
    let acc_iter = &mut accounts.iter();
    let admin_info = next_account_info(acc_iter)?;
    let price_info = next_account_info(acc_iter)?;
    let (price_pubkey, _bump_seed) = Price::get_price_pubkey_with_bump();

    if price_pubkey != *price_info.key {
        return Err(ProgramError::InvalidArgument);
    }

    if !admin_info.is_signer {
        return Err(ProgramError::MissingRequiredSignature);
    }

    // Updete price
    if price_info.data_is_empty() {
        return Err(ProcessingError::StoreNotInit.into());
    }

    let mut price = Price::try_from_slice(&price_info.data.borrow())?;
    if price.admin != admin_info.key.to_bytes() {
        return Err(ProcessingError::AccesDenited.into());
    }

    price.admin = admin_info.key.to_bytes();
    price.price = start_price;
    
    let _ = price.serialize(&mut &mut price_info.data.borrow_mut()[..]);
    msg!("Price is update!");
    Ok(())
}

pub fn buy_token (accounts: &[AccountInfo], amount: u64) -> ProgramResult {

    // Checking accounts
    let acc_iter = &mut accounts.iter();
    let user_info = next_account_info(acc_iter)?;
    let admin_info = next_account_info(acc_iter)?;
    let price_info = next_account_info(acc_iter)?;
    let (price_pubkey, bump_seed) = Price::get_price_pubkey_with_bump();
    
    if price_pubkey != *price_info.key {
        return Err(ProgramError::InvalidArgument);
    }

    if !user_info.is_signer {
        return Err(ProgramError::MissingRequiredSignature);
    }

    // Buy tocken
    if price_info.data_is_empty() {
        return Err(ProcessingError::StoreNotInit.into());
    }

    let price = Price::try_from_slice(&price_info.data.borrow())?;
    let cost = price.price * amount;

    msg!("Cost = {}", cost);

    invoke(
        &system_instruction::transfer( user_info.key, admin_info.key, cost),
        &[user_info.clone(), admin_info.clone()],
    )?;

    msg!("Buy is done!");
    Ok(())
}
