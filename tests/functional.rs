#![cfg(feature = "test-bpf")]
use std::env;

use borsh::{BorshDeserialize, BorshSerialize};
use test_task::{entrypoint::process_instruction, id, instruction::StoreInstruction};
use test_task::{
    state::Price,
};
use solana_program::pubkey::Pubkey;
use solana_program::system_instruction;
use solana_program_test::{processor, tokio, ProgramTest, ProgramTestContext};
use solana_sdk::signature::{Keypair, Signer};
use solana_sdk::transaction::Transaction;

struct Env {
    ctx: ProgramTestContext,
    admin: Keypair,
    user: Keypair,
}

impl Env {
    async fn new() -> Self {
        let program_test = ProgramTest::new("test_task", id(), processor!(process_instruction));
        let mut ctx = program_test.start_with_context().await;

        let admin = Keypair::new();
        let user = Keypair::new();

        // credit admin and user accounts
        ctx.banks_client
            .process_transaction(Transaction::new_signed_with_payer(
                &[
                    system_instruction::transfer(
                        &ctx.payer.pubkey(),
                        &admin.pubkey(),
                        1_000_000_000,
                    ),
                    system_instruction::transfer(
                        &ctx.payer.pubkey(),
                        &user.pubkey(),
                        1_000_000_000,
                    ),
                ],
                Some(&ctx.payer.pubkey()),
                &[&ctx.payer],
                ctx.last_blockhash,
            ))
            .await
            .unwrap();

        // init store 
        let tx = Transaction::new_signed_with_payer(
            &[StoreInstruction::init(
                &admin.pubkey(),
                5,
            )],
            Some(&admin.pubkey()),
            &[&admin],
            ctx.last_blockhash,
        );
        ctx.banks_client.process_transaction(tx).await.unwrap();
        let acc =
            ctx.banks_client.get_account(Price::get_price_pubkey()).await.unwrap().unwrap();
        let price = Price::try_from_slice(acc.data.as_slice()).unwrap();
        assert_eq!(price.admin, admin.pubkey().to_bytes());
        assert_eq!(price.price, 5);

        Env { ctx, admin, user }
    }
}

#[tokio::test]
async fn init_test() {
    let mut env = Env::new().await;
}

#[tokio::test]
async fn update_test() {
    let mut env = Env::new().await;

    let tx = Transaction::new_signed_with_payer(
        &[StoreInstruction::update(
            &env.admin.pubkey(), 
            10
        )],
        Some(&env.admin.pubkey()),
        &[&env.admin],
        env.ctx.last_blockhash,
    );

    env.ctx.banks_client.process_transaction(tx).await.unwrap();

    let acc = env
    .ctx
    .banks_client
    .get_account(Price::get_price_pubkey())
    .await
    .unwrap()
    .unwrap();
    let price = Price::try_from_slice(acc.data.as_slice()).unwrap();
    assert_eq!(price.admin, env.admin.pubkey().to_bytes());
    assert_eq!(price.price, 10);
}

#[tokio::test]
async fn buy_test() {
    let mut env = Env::new().await;

    let amout: u64 = 20_000;

    let tx = Transaction::new_signed_with_payer(
        &[StoreInstruction::buy(
            &env.user.pubkey(),
            &env.admin.pubkey(), 
            amout
        )],
        Some(&env.user.pubkey()),
        &[&env.user],
        env.ctx.last_blockhash,
    );

    env.ctx.banks_client.process_transaction(tx).await.unwrap();

    let balance_user = env
        .ctx
        .banks_client
        . get_balance(env.user.pubkey())
        .await
        .unwrap();

    let balance_admin = env
        .ctx
        .banks_client
        . get_balance(env.admin.pubkey())
        .await
        .unwrap();

    assert_eq!(balance_user, 1_000_000_000 - 5_000 - amout * 5);
    assert_eq!(balance_admin, 1_000_000_000 - 1_174_280 + amout * 5);
}